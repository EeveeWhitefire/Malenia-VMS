#include "../headers/Hikvision.h"

namespace Hikvision
{
    SDK_Device::SDK_Device(SDK_LOGIN_MODE loginMode, const char* ip, const int port) : SDKDevice(loginMode, ip, port)
    {
    }

    SDK_Device::~SDK_Device()
    {
        NET_DVR_Logout_V30(this->_userId);
    }

    int SDK_Device::login(const char* username, const char* password)
    {
        IChannel* child = NULL;
        time_t now = 0;
        tm* ltm = NULL;
        NET_DVR_USER_LOGIN_INFO loginRequestIn;
        NET_DVR_DEVICEINFO_V40 infoRequestIn;
        NET_DVR_TIME datetimeRequestOut;
        NET_DVR_IPPARACFG_V40 channelsRequestInfo;
        DWORD nRetLen = 0;
        int i = 0, temp = 0;

        // initiating the structs with zeros
        memset(&loginRequestIn, 0, sizeof(NET_DVR_USER_LOGIN_INFO));
        memset(&infoRequestIn, 0, sizeof(NET_DVR_DEVICEINFO_V40));
        memset(&datetimeRequestOut, 0, sizeof(NET_DVR_TIME));
        memset(&channelsRequestInfo, 0, sizeof(NET_DVR_IPPARACFG_V40));

        loginRequestIn.bUseAsynLogin = false;
        loginRequestIn.wPort = this->_port;
        strncpy_t(loginRequestIn.sDeviceAddress, NET_DVR_DEV_ADDRESS_MAX_LEN, this->_ip, MAX_IP_LEN);
        strncpy_t(loginRequestIn.sUserName, MAX_SHORT_STR_LEN, username, MAX_SHORT_STR_LEN);
        strncpy_t(loginRequestIn.sPassword, MAX_SHORT_STR_LEN, password, MAX_SHORT_STR_LEN);

        // 0-Private, 1-ISAPI, 2-Self-adaptive (ISAPI or private login, may affect the performance).
        if(this->_loginMode == LOGIN_MODE_SDK)
            loginRequestIn.byLoginMode = 0;
        else if(this->_loginMode == LOGIN_MODE_HTTP)
            loginRequestIn.byLoginMode = 1;

        this->_userId = NET_DVR_Login_V40(&loginRequestIn, &infoRequestIn);
        if(this->_userId >= 0)
        {
            temp = strnlen(reinterpret_cast<const char*>(infoRequestIn.struDeviceV30.sSerialNumber), sizeof(infoRequestIn.struDeviceV30.sSerialNumber)) - 27; // 27 is the size of the serial number
            for(i = 0; i < temp; i++)
                this->_modelName[i] = infoRequestIn.struDeviceV30.sSerialNumber[i];

            strncpy_t(this->_serialNumber, MAX_SHORT_STR_LEN, reinterpret_cast<const char*>(infoRequestIn.struDeviceV30.sSerialNumber + temp), MAX_SHORT_STR_LEN);

            // does it support ip channels?
            if (NET_DVR_GetDVRConfig(this->_userId, NET_DVR_GET_IPPARACFG_V40, 0, &channelsRequestInfo, sizeof(NET_DVR_IPPARACFG_V40), &nRetLen))
            {
                // analog channels
                for (i = 0; i < (int)channelsRequestInfo.dwAChanNum; i++)
                {
                    if (channelsRequestInfo.byAnalogChanEnable[i])
                    {
                        this->_channelsAmount++;
                        this->_chIds[this->_channelsAmount] = i + infoRequestIn.struDeviceV30.byStartChan;
                    }
                }

                // ip channels
                for (i = 0; i < (int)channelsRequestInfo.dwDChanNum; i++)
                {
                    if(channelsRequestInfo.struIPDevInfo[i].byEnable)
                    {
                        this->_channelsAmount++;
                        this->_chIds[this->_channelsAmount] = i + channelsRequestInfo.dwStartDChan;
                    }
                }
            }
            else
            {
                if(infoRequestIn.struDeviceV30.byChanNum == 0)
                    MaleniaException::show(ERR_NO_CHANNELS_FOUND, "This device seems to have the IP channels feature disabled");
                else
                {
                    for (i = 0; i < (int)infoRequestIn.struDeviceV30.byChanNum; i++)
                    {
                        this->_channelsAmount++;
                        this->_chIds[this->_channelsAmount] = i + infoRequestIn.struDeviceV30.byStartChan;
                    }
                }
            }

            now = time(0);
            ltm = localtime(&now);
            if(NET_DVR_GetDVRConfig(this->_userId,  NET_DVR_GET_TIMECFG, 0, &datetimeRequestOut, sizeof(NET_DVR_TIME), &nRetLen))
                this->_dtOffset = calc_datetime_offset(ltm, datetimeRequestOut.dwYear, datetimeRequestOut.dwMonth, datetimeRequestOut.dwDay, 
                                                       datetimeRequestOut.dwHour, datetimeRequestOut.dwMinute, datetimeRequestOut.dwSecond);

            for(i = 0; i < this->_channelsAmount; i++)
            {
                child = new SDK_Channel(this->_userId, this->_chIds[i + 1], this);
                child->setText(0, QString("CH %1").arg(i + 1));
                this->_channels.push_back(child);
            }

            this->updateCredentials(username, password);
            return 1;
        }
        else
        {
            switch(NET_DVR_GetLastError())
            {
                case NET_DVR_PASSWORD_ERROR:
                    MaleniaException::show(LOGIN_RET_INVALID_CREDENTIALS);
                    break;
                case NET_DVR_NETWORK_FAIL_CONNECT:
                    MaleniaException::show(LOGIN_RET_TIMEOUT);
                    break;
                default:
                    MaleniaException::show(LOGIN_RET_UNKNOWN, std::to_string(NET_DVR_GetLastError()).c_str());
            }
        }

        return 0;
    }

    SDK_Channel::SDK_Channel(int userId, int chid, QTreeWidgetItem* parent) : IChannel(chid, parent)
    {
        this->_userId = userId;
    }

    SDK_Channel::~SDK_Channel()
    {

    }

    IStream* SDK_Channel::StartLiveView(QFrame* qframe)
    {
        IStream* stream = this->StartStream(new SDK_LiveStream(this->_userId, this->_chid, this), qframe);
        if(stream != NULL)
            this->_liveStream = stream;

        return stream;
    }

    IStream* SDK_Channel::StartPlayback(datetime from, datetime to, QFrame* qframe)
    {
        return this->StartStream(new SDK_PlaybackStream(this->_userId, this->_chid, 
                                                        calc_datetime_difference(from, this->_dtOffset), 
                                                        calc_datetime_difference(to, this->_dtOffset), 
                                                        this), qframe);
    }

    SDK_LiveStream::SDK_LiveStream(int userId, int chid, IChannel* channel, QWidget* parent) : IStream(channel, parent)
    {
        this->_userId = userId;
        this->_chid = chid;
    }

    SDK_LiveStream::~SDK_LiveStream()
    {
        this->Stop();
    }

    int SDK_LiveStream::Start(QFrame* qframe)
    {
        NET_DVR_PREVIEWINFO liveRequestIn;
        memset(&liveRequestIn, 0, sizeof(NET_DVR_PREVIEWINFO));

        liveRequestIn.lChannel        = this->_chid;  //channel NO
        liveRequestIn.dwLinkMode      = 0; // 0- TCP, 1- UDP, 2- multicast, 3- RTP, 4-RTP/RTSP, 5-RSTP/HTTP, 6- HRUDP
        liveRequestIn.dwStreamType    = 1; // 0-main stream, 1-sub stream, 2-stream 3, 3- virtual stream, and so on
        liveRequestIn.hPlayWnd        = (HWND)qframe->winId();
        liveRequestIn.bBlocked        = 0;
        liveRequestIn.dwDisplayBufNum = 6;

        // if(m_streamKey != NULL)
            // strcpy((char*)liveRequestIn.byStreamID, m_streamKey);

        //Set callback function of getting stream.
        this->_handle = NET_DVR_RealPlay_V40(this->_userId, &liveRequestIn, NULL, NULL);
        if (this->_handle < 0)
        {
            switch(NET_DVR_GetLastError())
            {
                case NET_DVR_LOADPLAYERSDKFAILED:
                    MaleniaException::show(ERR_UNKNOWN, "Failed to load SDK");
                    break;
                default:
                    MaleniaException::show(ERR_UNKNOWN, std::to_string(NET_DVR_GetLastError()).c_str());
                    break;
            }
            return 0;
        }
        
        return IStream::Start(qframe);
    }

    void SDK_LiveStream::Stop()
    {
        NET_DVR_StopRealPlay(this->_handle);
        this->_handle = HANDLE_RESET;

        IStream::Stop();
    }

    int SDK_LiveStream::Replay()
    {
        return NET_DVR_ChangeWndResolution(this->_handle);
    }

    SDK_PlaybackStream::SDK_PlaybackStream(int userId, int chid, datetime from, datetime to, IChannel* channel, QWidget* parent) : Playback_Stream(from, to, channel, parent)
    {
        this->_userId = userId;
        this->_chid = chid;
    }

    SDK_PlaybackStream::~SDK_PlaybackStream()
    {
        this->Stop();
    }

    int SDK_PlaybackStream::Start(QFrame* qframe)
    {
        NET_DVR_VOD_PARA playbackRequestIn;

        // initiating the struct with zeros
        memset(&playbackRequestIn, 0, sizeof(NET_DVR_VOD_PARA));

        playbackRequestIn.hWnd = (HWND)qframe->winId();
        playbackRequestIn.struIDInfo.dwChannel = this->_chid;
        playbackRequestIn.byStreamType = 0; // Stream type: 0-main stream, 1-sub-stream, 2-third stream. This parameter is not supported by ISAPI protocol.
        
        playbackRequestIn.struBeginTime.dwYear = this->_from.year;
        playbackRequestIn.struBeginTime.dwMonth = this->_from.month;
        playbackRequestIn.struBeginTime.dwDay = this->_from.day;
        playbackRequestIn.struBeginTime.dwHour = this->_from.hours;
        playbackRequestIn.struBeginTime.dwMinute = this->_from.minutes;
        playbackRequestIn.struBeginTime.dwSecond = this->_from.seconds;

        playbackRequestIn.struEndTime.dwYear = this->_to.year;
        playbackRequestIn.struEndTime.dwMonth = this->_to.month;
        playbackRequestIn.struEndTime.dwDay = this->_to.day;
        playbackRequestIn.struEndTime.dwHour = this->_to.hours;
        playbackRequestIn.struEndTime.dwMinute = this->_to.minutes;
        playbackRequestIn.struEndTime.dwSecond = this->_to.seconds;

        this->_handle = NET_DVR_PlayBackByTime_V40(this->_userId, &playbackRequestIn);
        if(this->_handle < 0)
        {
            switch(NET_DVR_GetLastError())
            {
                case NET_DVR_PARAMETER_ERROR:
                    MaleniaException::show(ERR_INVALID_INPUT, 
                                           QObject::tr("Invalid range %1:%2 %3/%4/%5 - %6:%7 %8/%9/%10").arg(this->_from.hours)
                                                                                                        .arg(this->_from.minutes)
                                                                                                        .arg(this->_from.day)
                                                                                                        .arg(this->_from.month)
                                                                                                        .arg(this->_from.year)
                                                                                                        .arg(this->_to.hours)
                                                                                                        .arg(this->_to.minutes)
                                                                                                        .arg(this->_to.day)
                                                                                                        .arg(this->_to.month)
                                                                                                        .arg(this->_to.year)
                                                                                                        .toLocal8Bit().data());
                    break;
                default:
                    QMessageBox::information(NULL, QObject::tr("NET_DVR_PlayBackByTime_V40 error"), QObject::tr("error code : %1").arg(NET_DVR_GetLastError()));
                    break;
            }
            return 0;
        }

        if(NET_DVR_PlayBackControl_V40(this->_handle, NET_DVR_PLAYSTART, NULL, 0, NULL, 0))
            return IStream::Start(qframe);
        else
        {
            this->Stop();
            return 0;
        }
    }

    void SDK_PlaybackStream::TogglePause()
    {
        if(this->_isPlaying)
            NET_DVR_PlayBackControl_V40(this->_handle, NET_DVR_PLAYPAUSE, NULL, 0, NULL, 0);
        else
            NET_DVR_PlayBackControl_V40(this->_handle, NET_DVR_PLAYRESTART, NULL, 0, NULL, 0);

        Playback_Stream::TogglePause();
    }

    void SDK_PlaybackStream::Stop()
    {
        NET_DVR_StopPlayBack(this->_handle);
        this->_handle = HANDLE_RESET;

        IStream::Stop();
    }

    int SDK_PlaybackStream::Replay()
    {
        return NET_DVR_PlayBackControl_V40(this->_handle, NET_DVR_CHANGEWNDRESOLUTION, NULL, 0, NULL, 0);
    }
}

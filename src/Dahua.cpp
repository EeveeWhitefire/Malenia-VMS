#include "../headers/Dahua.h"
#include "../headers/external/dhplay.h"

namespace Dahua
{
    SDK_Device::SDK_Device(SDK_LOGIN_MODE loginMode, const char* ip, const int port) : SDKDevice(loginMode, ip, port)
    {
    }

    SDK_Device::~SDK_Device()
    {
        if(this->_userId != 0)
            CLIENT_Logout(this->_userId);
    }

    int SDK_Device::login(const char* username, const char* password)
    {
        IChannel* child = NULL;
        time_t now = 0;
        tm* ltm = NULL;
        NET_IN_LOGIN_WITH_HIGHLEVEL_SECURITY loginRequestIn;
        NET_OUT_LOGIN_WITH_HIGHLEVEL_SECURITY loginRequestOut;
        NET_DEV_CHN_COUNT_INFO channelsRequestIn;
        NET_TIME datetimeRequestOut;
        DWORD nRetLen = 0;
        int i = 0;

        // initiating the structs with zeros
        memset(&loginRequestIn, 0, sizeof(NET_IN_LOGIN_WITH_HIGHLEVEL_SECURITY));
        memset(&loginRequestOut, 0, sizeof(NET_OUT_LOGIN_WITH_HIGHLEVEL_SECURITY));
        memset(&channelsRequestIn, 0, sizeof(NET_DEV_CHN_COUNT_INFO));
        memset(&datetimeRequestOut, 0, sizeof(NET_TIME));

        loginRequestIn.dwSize = sizeof(loginRequestIn);
        strncpy_t(loginRequestIn.szIP, MAX_IP_LEN, this->_ip, MAX_IP_LEN);
        strncpy_t(loginRequestIn.szUserName, MAX_SHORT_STR_LEN, username, MAX_SHORT_STR_LEN);
        strncpy_t(loginRequestIn.szPassword, MAX_SHORT_STR_LEN, password, MAX_SHORT_STR_LEN);
        loginRequestIn.nPort = this->_port;

        if(this->_loginMode == LOGIN_MODE_SDK)
            loginRequestIn.emSpecCap = EM_LOGIN_SPEC_CAP_TCP;
        else if(this->_loginMode == LOGIN_MODE_HTTP)
            loginRequestIn.emSpecCap = EM_LOGIN_SPEC_CAP_P2P;

        loginRequestOut.dwSize = sizeof(loginRequestOut);
        loginRequestOut.nError = -1;
 
        this->_userId = CLIENT_LoginWithHighLevelSecurity(&loginRequestIn, &loginRequestOut);
        if(this->_userId == 0)
        {
            switch(loginRequestOut.nError)
            {
                case 0:
                    break;
                case 1:
                    MaleniaException::show(LOGIN_RET_INVALID_CREDENTIALS);
                    break;
                case 2:
                    MaleniaException::show(LOGIN_RET_INVALID_USERNAME);
                    break;
                case 3:
                    MaleniaException::show(LOGIN_RET_TIMEOUT);
                    break;
                case 4: //Repeat Login
                    MaleniaException::show(LOGIN_RET_UNKNOWN);
                    break;
                case 5:
                    MaleniaException::show(LOGIN_RET_LOCKED);
                    break;
                case 6: //User in black list
                    MaleniaException::show(LOGIN_RET_INVALID_USERNAME);
                    break;
                case 7:
                    MaleniaException::show(LOGIN_RET_DEVICE_BUSY);
                    break;
                case 8: //Sub Connect Failed
                    MaleniaException::show(LOGIN_RET_UNKNOWN);
                    break;
                case 9: //Host Connect Failed
                    MaleniaException::show(LOGIN_RET_TIMEOUT);
                    break;
                case 10 :
                    MaleniaException::show(LOGIN_RET_MAX_CONNECTIONS);
                    break;
                case 11: //Support Protocol3 Only
                    MaleniaException::show(LOGIN_RET_UNKNOWN);
                    break;
                case 12: //UKey Info Error
                    MaleniaException::show(LOGIN_RET_UNKNOWN);
                    break;
                case 13: //No Authorized
                    MaleniaException::show(LOGIN_RET_UNKNOWN);
                    break;
                case 18: //Device Account isn't Initialized
                    MaleniaException::show(LOGIN_RET_INVALID_USERNAME);
                    break;
                default:
                    MaleniaException::show(LOGIN_RET_UNKNOWN);
                    break;
            }

            return 0;
        }

        // calculating the datetime difference between ours and the NVR's
        now = time(0);
        ltm = localtime(&now);
        if(CLIENT_GetDevConfig(this->_userId, DH_DEV_TIMECFG, NULL, &datetimeRequestOut, sizeof(NET_TIME), &nRetLen)) // getting time setting of device
                this->_dtOffset = calc_datetime_offset(ltm, datetimeRequestOut.dwYear, datetimeRequestOut.dwMonth, datetimeRequestOut.dwDay, 
                                                       datetimeRequestOut.dwHour, datetimeRequestOut.dwMinute, datetimeRequestOut.dwSecond);

        channelsRequestIn.stuVideoIn.dwSize = sizeof(channelsRequestIn.stuVideoIn);
        channelsRequestIn.stuVideoOut.dwSize = sizeof(channelsRequestIn.stuVideoOut);
        if(CLIENT_QueryDevState(this->_userId, DH_DEVSTATE_DEV_CHN_COUNT, (char*)&channelsRequestIn, channelsRequestIn.dwSize, &i)) // getting number of channels
            this->_channelsAmount = channelsRequestIn.stuVideoOut.nMaxTotal;
        else
            this->_channelsAmount = loginRequestOut.stuDeviceInfo.nChanNum;

        for(i = 0; i < this->_channelsAmount; i++)
        {
            child = new SDK_Channel(this->_userId, i, this);
            child->setText(0, QString("CH %1").arg(i + 1));
            this->_channels.push_back(child);
        }

        this->updateCredentials(username, password);
        return 1;
    }

    SDK_Channel::SDK_Channel(LLONG userId, int chid, QTreeWidgetItem* parent) : IChannel(chid, parent)
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

    SDK_LiveStream::SDK_LiveStream(LLONG userId, int chid, IChannel* channel, QWidget* parent) : IStream(channel, parent)
    {
        this->_userId = userId;
        this->_chid = chid;
    }

    SDK_LiveStream::~SDK_LiveStream()
    {
        this->Stop();
    }

    void CALLBACK LiveCallback(LLONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, LONG param, LDWORD dwUser) { PLAY_InputData((int)dwUser, pBuffer, dwBufSize); }
    int SDK_LiveStream::Start(QFrame* qframe)
    {
        PLAY_GetFreePort(&this->_streamingPort);
        PLAY_OpenStream(this->_streamingPort, NULL, 0, 900*1024);
        PLAY_Play(this->_streamingPort, (void*)qframe->winId()); // for some reason dahua wants a void*
                                     
        this->_handle = CLIENT_StartRealPlay(this->_userId, this->_chid, NULL, DH_RType_Realplay_1, // DH_RType_Realplay - main stream, DH_RType_Realplay_1 - sub stream 1
                                             LiveCallback, NULL, (LDWORD)this->_streamingPort);
        if (this->_handle == 0)
        {
            switch(CLIENT_GetLastError())
            {
                case NET_NETWORK_ERROR:
                    MaleniaException::show(ERR_NETWORK_ERROR, "A protocol error has occured. It may result from network timeout");
                    break;
                case NET_ERROR_GETCFG_SESSION:
                    MaleniaException::show(ERR_NETWORK_ERROR, "Failed to get connection session information");
                    break;
                default:
                    MaleniaException::show(ERR_UNKNOWN, std::to_string(CLIENT_GetLastError() - 0x80000000).c_str());
                    break;
            }

            return 0;
        }

        return IStream::Start(qframe);
    }

    void SDK_LiveStream::Stop()
    {
        CLIENT_StopRealPlay(this->_handle);
        PLAY_Stop(this->_streamingPort);
        PLAY_CloseStream(this->_streamingPort);
        PLAY_ReleasePort(this->_streamingPort);
        this->_handle = HANDLE_RESET;
        IStream::Stop();
    }

    int SDK_LiveStream::Replay()
    {
        return 1;
    }

    SDK_PlaybackStream::SDK_PlaybackStream(LLONG userId, int chid, datetime from, datetime to, IChannel* channel, QWidget* parent) : Playback_Stream(from, to, channel, parent)
    {
        this->_userId = userId;
        this->_chid = chid;
    }

    SDK_PlaybackStream::~SDK_PlaybackStream()
    {
        this->Stop();
    }
    
    int CALLBACK PlaybackCallback(LLONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, LDWORD dwUser) { PLAY_InputData((int)dwUser, pBuffer, dwBufSize); return 1; }
    int SDK_PlaybackStream::Start(QFrame* qframe)
    {
        NET_TIME playbackRequestInFrom, playbackRequestInTo;
        memset(&playbackRequestInFrom, 0, sizeof(NET_TIME));
        memset(&playbackRequestInTo, 0, sizeof(NET_TIME));

        playbackRequestInFrom.dwYear = this->_from.year;
        playbackRequestInFrom.dwMonth = this->_from.month;
        playbackRequestInFrom.dwDay = this->_from.day;
        playbackRequestInFrom.dwHour = this->_from.hours;
        playbackRequestInFrom.dwMinute = this->_from.minutes;
        playbackRequestInFrom.dwSecond = this->_from.seconds;

        playbackRequestInTo.dwYear = this->_to.year;
        playbackRequestInTo.dwMonth = this->_to.month;
        playbackRequestInTo.dwDay = this->_to.day;
        playbackRequestInTo.dwHour = this->_to.hours;
        playbackRequestInTo.dwMinute = this->_to.minutes;
        playbackRequestInTo.dwSecond = this->_to.seconds;

        PLAY_GetFreePort(&this->_streamingPort);
        PLAY_OpenStream(this->_streamingPort, NULL, 0, 900*1024);
        PLAY_Play(this->_streamingPort, (void*)qframe->winId()); // for some reason dahua wants a void*

		// CLIENT_PlayBackControl
        this->_handle = CLIENT_PlayBackByTimeEx(this->_userId, this->_chid, &playbackRequestInFrom, &playbackRequestInTo, (HWND)qframe->winId(), NULL, NULL, 
                                                PlaybackCallback, (DWORD)this->_streamingPort);
        if(this->_handle == 0)
        {
            switch(CLIENT_GetLastError())
            {
                case NET_ILLEGAL_PARAM:
                    MaleniaException::show(ERR_INVALID_INPUT);
                    break;
                case NET_NOT_SAVING:
                    MaleniaException::show(ERR_PLAYBACK_NOT_EXIST);
                    break;
                case NET_NO_RECORD_FOUND:
                    MaleniaException::show(ERR_PLAYBACK_NOT_EXIST);
                    break;
                default:
                    MaleniaException::show(ERR_UNKNOWN, std::to_string(CLIENT_GetLastError() - 0x80000000).c_str());
                    break;
            }
            return 0;
        }

        return IStream::Start(qframe);
    }

    void SDK_PlaybackStream::TogglePause()
    {
        CLIENT_PausePlayBack(this->_handle, this->_isPlaying ? 1 : 0); // 1 - to pause, 0 - to resume
        Playback_Stream::TogglePause();
    }

    void SDK_PlaybackStream::Stop()
    {
        CLIENT_StopPlayBack(this->_handle);
        PLAY_Stop(this->_streamingPort);
        PLAY_CloseStream(this->_streamingPort);
        PLAY_ReleasePort(this->_streamingPort);
        this->_handle = HANDLE_RESET;

        IStream::Stop();
    }

    int SDK_PlaybackStream::Replay()
    {
        return 1;
    }
}

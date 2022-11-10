#include <sys/socket.h>
#include <arpa/inet.h>

#include "../headers/Provision.h"
#include "../headers/external/pugixml.hpp"
#include "../headers/external/CkHttp/CkHttpResponse.h"

namespace Provision
{
    std::string FORMAT_RTSP_LIVE(const std::string rtspUri, int camId)
    {
        return rtspUri + "/chID=" + std::to_string(camId) + "&profile1?transportmode=mcast"; 
    }

    std::string FORMAT_RTSP_PLAYBACK(const std::string rtspUri, int camId, datetime start, datetime stop)
    {
        return rtspUri + "/chID=" + std::to_string(camId) + "&profile1?transportmode=mcast";    
    }

    HTTP_Device::HTTP_Device(const char* ip, const int port) : HTTPDevice(ip, port)
    {
    }

    HTTP_Device::~HTTP_Device()
    {
    }

    int HTTP_Device::login(const char* username, const char* password)
    {
        std::string uri = this->_uriBegin + "/Scripts/base.js";
        char* response = (char*)this->_http.quickGetStr(uri.c_str());
        char* strtokBuffer = strtok(response, "\r\n");
        std::string strtokBufferStr, protocolVersion, systemType, xml;
        int i = 0, status = 0;
        pugi::xml_document doc;
        pugi::xml_parse_result result;
        pugi::xml_node root;
        IChannel* child = NULL;

        while(strtokBuffer != NULL)
        {
            strtokBufferStr = strtokBuffer;
            if(strtokBufferStr.find("var protocolVer") != std::string::npos)
                break;
            strtokBuffer = strtok(NULL, "\r\n");
        }

        if(strtokBuffer != NULL)
        {
            for(i = 0; i < (int)strnlen(strtokBuffer, MAX_STR_LEN); i++)
            {
                if(status == 0 && strtokBuffer[i] == '=')
                    status = 1;
                else if(status == 1)
                {
                    if(strtokBuffer[i] == ',')
                        status = 2;
                    else if(strtokBuffer[i] != ' ' && strtokBuffer[i] != '"')
                        protocolVersion = protocolVersion + strtokBuffer[i];
                }
                else if(status == 2 && strtokBuffer[i] == '=')
                    status = 3;
                else if(status == 3)
                {
                    if(strtokBuffer[i] == ',')
                        status = 2;
                    else if(strtokBuffer[i] != ' ' && strtokBuffer[i] != '"' && strtokBuffer[i] != ';')
                        systemType = systemType + strtokBuffer[i];
                }
            }

            xml = "<?xml version=\"1.0\" encoding=\"utf-8\"?><request version=\"" + protocolVersion + "\" systemType=\"" + systemType + "\" clientType=\"WEB\"/>";
            this->_http.put_Login(username);
            this->_http.put_Password(password);
            this->_http.put_BasicAuth(1);
            
            uri = this->_uriBegin + "/doLogin";
            response = (char*)this->_http.PostXml(uri.c_str(), xml.c_str(), "utf-8")->bodyStr();
            if(std::string(response).find("success") != std::string::npos && std::string(response).find("fail") == std::string::npos)
            {
                uri = this->_uriBegin + "/queryOnlineChlList";
                response = (char*)this->_http.PostXml(uri.c_str(), xml.c_str(), "utf-8")->bodyStr();

                result = doc.load_string(response);
                if(result)
                {
                    root = doc.child("response").child("content");
                    this->_channelsAmount = std::distance(root.children("item").begin(), root.children("item").end());

                    uri = this->_uriBegin + "/queryBasicCfg";
                    response = (char*)this->_http.PostXml(uri.c_str(), xml.c_str(), "utf-8")->bodyStr();

                    result = doc.load_string(response);
                    if(result)
                    { 
                        root = doc.child("response").child("content");
                        strncpy_t(this->_serialNumber, MAX_SHORT_STR_LEN, root.child_value("sn"), MAX_SHORT_STR_LEN);
                        strncpy_t(this->_modelName, MAX_SHORT_STR_LEN, root.child_value("productModel"), MAX_SHORT_STR_LEN);

                        this->updateCredentials(username, password);

                        for(i = 0; i < this->_channelsAmount; i++)
                        {
                            child = new HTTP_Channel(this->_rtspUriBegin, i + 1, this);
                            child->setText(0, QString("CH %1").arg(i + 1));
                            this->_channels.push_back(child);
                        }
                        return 1;
                    }
                    else
                        MaleniaException::show(LOGIN_RET_UNKNOWN);
                }
                else
                    MaleniaException::show(LOGIN_RET_UNKNOWN);
            }
            else
                MaleniaException::show(LOGIN_RET_INVALID_CREDENTIALS);
        }
        else
            MaleniaException::show(LOGIN_RET_UNKNOWN);

        return 0;
    }

    HTTP_Channel::HTTP_Channel(std::string rtspUriBegin, int chid, QTreeWidgetItem* parent) : RTSPChannel(rtspUriBegin, chid, parent)
    {

    }

    HTTP_Channel::~HTTP_Channel()
    {

    }

    IStream* HTTP_Channel::StartLiveView(QFrame* qframe)
    {
        return this->StartStream(new RTSP_Stream(FORMAT_RTSP_LIVE(this->_rtspUriBegin, this->_chid), this), qframe);
    }

    IStream* HTTP_Channel::StartPlayback(datetime from, datetime to, QFrame* qframe)
    {
        return this->StartStream(new RTSP_Stream(FORMAT_RTSP_PLAYBACK(this->_rtspUriBegin, this->_chid, from, to), this), qframe);
    }

    SDK_Device::SDK_Device(const char* ip, const int port) : IDevice(ip, port)
    {
    }

    SDK_Device::~SDK_Device()
    {
        if(this->_sockfd > 0)
            close(this->_sockfd);
    }

    int SDK_Device::login(const char* username, const char* password)
    {
        struct sockaddr_in addrinfo;
        int recvsize = 0, sendsize = 0, i = 0, 
            usernameLen = strnlen(username, MAX_SHORT_STR_LEN), passwordLen = strnlen(password, MAX_SHORT_STR_LEN), ipLen = strnlen(this->_ip, MAX_IP_LEN);
        char buffer[SOCKET_BUFFER_SIZE] = {0};
        pugi::xml_document doc;
        pugi::xml_parse_result result;
        pugi::xml_node root;
        IChannel* child = NULL;

        memset(&addrinfo, 0, sizeof(addrinfo)); 
        addrinfo.sin_family = AF_INET;
        addrinfo.sin_port = htons(this->_port);
        addrinfo.sin_addr.s_addr = inet_addr(this->_ip);
        
        if((this->_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
            MaleniaException::show(LOGIN_RET_UNKNOWN, "Failed to create socket");
        else if(connect(this->_sockfd, (sockaddr*)&addrinfo, sizeof(addrinfo)) == -1)
            MaleniaException::show(LOGIN_RET_PING_FAILED);
        else
        {
            recvsize = recv(this->_sockfd, buffer, SOCKET_BUFFER_SIZE, 0);
            if(recvsize > 0)
            {
                if(bytes_contain(buffer, SOCKET_BUFFER_SIZE, "P*") || bytes_contain(buffer, SOCKET_BUFFER_SIZE, "A*")) 
                {
                    memset(buffer, 0, recvsize);

                    memcpy(buffer + 16*0, "\x31\x31\x31\x31\x88\x00\x00\x00\x01\x01\x00\x00\x9b\x83\x00\x00", 16);
                    memcpy(buffer + 16*1, "\x01\x00\x00\x00\x78\x00\x00\x00\x05\x00\x00\x00\x00\x00\x00\x00", 16);
                    memcpy(buffer + 16*2, username, usernameLen);

                    sendsize = 16*2 + usernameLen;
                    memcpy(buffer + sendsize, "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 16);
                    memcpy(buffer + sendsize + 16, "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 15);
                    memcpy(buffer + sendsize + 16 + 15, password, passwordLen);

                    sendsize = sendsize + 16 + 15 + passwordLen;
                    memcpy(buffer + sendsize + 16*0, "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 16);
                    memcpy(buffer + sendsize + 16*1, "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 16);
                    memcpy(buffer + sendsize + 16*2, "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 16);
                    memcpy(buffer + sendsize + 16*3, "\x00\x00\x00\x00\x00\x00", 6);
                    memcpy(buffer + sendsize + 16*3 + 6, get_mac_address().c_str(), 8); // half of mac address
                    memcpy(buffer + sendsize + 16*3 + 6 + 8, "\x04\x00\x00\x00\x31\x31\x31\x31", 8);

                    sendsize = sendsize + 16*3 + 6 + 8 + 8;
                }
                else if(bytes_contain(buffer, SOCKET_BUFFER_SIZE, "S") || bytes_contain(buffer, SOCKET_BUFFER_SIZE, "Vx") || 
                        bytes_contain(buffer, SOCKET_BUFFER_SIZE, "VW") || bytes_contain(buffer, SOCKET_BUFFER_SIZE, "Z") || 
                        bytes_contain(buffer, SOCKET_BUFFER_SIZE, "t"))
                {
                    memset(buffer, 0, recvsize);

                    memcpy(buffer + 16*0, "\x31\x31\x31\x31\xf8\x02\x00\x00\x04\x00\x00\x01\x01\x01\x00\x00", 16);
                    memcpy(buffer + 16*1, "\x2d\x01\x00\x00\xe8\x02\x00\x00\x02\x00\x00\x00\x89\x59\xbd\x62", 16);
                    memcpy(buffer + 16*2, "\x78\x00\x13\x4d\x87\x8b\x98\x7f\x47\x45\xe4\x48\x9d\x7f\x5e\x63", 16);
                    memcpy(buffer + 16*3, "\x5e\x02\xfd\x4f\xb0\xe0\xc3\xe4\x75\x42\x94\x4c", 12);
                    memcpy(buffer + 16*3 + 12, username, usernameLen);
                    sendsize = 16*3 + 12 + 16*4; // some leading zeros
                    
                    memcpy(buffer + sendsize, password, passwordLen);
                    sendsize = sendsize + 16*4; // some leading zeros

                    memcpy(buffer + sendsize, this->_ip, ipLen);
                    sendsize = sendsize + 16*23 + 8 + 3; // some leading zeros
                                                         
                    memcpy(buffer + sendsize, "\x31\x37\x38\x42\x46\x42\x46\x46\x30\x30\x30\x41\x30\x36\x35\x35\x42\x33\x45\x38\x36\x30\x43\x44\x36\x31\x37\x38", 28);
                    sendsize = 768; //fixed size
                }
                else if(bytes_contain(buffer, SOCKET_BUFFER_SIZE, "head"))
                {
                    memset(buffer, 0, recvsize);

                    memcpy(buffer + 16*0, "\x31\x31\x31\x31\x88\x00\x00\x00\x01\x01\x00\x00\x08\x26\xdd\x33", 16);
                    memcpy(buffer + 16*1, "\x3e\x0a\x3c\x63\x78\x00\x00\x00\x03\x00\x00\x00\x00\x00\x00\x00", 16);
                    memcpy(buffer + 16*2, username, usernameLen);

                    sendsize = 16*2 + usernameLen;
                    memcpy(buffer + sendsize + 16*0, "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 16);
                    memcpy(buffer + sendsize + 16*1, "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 15);
                    memcpy(buffer + sendsize + 16*1 + 15, password, passwordLen);

                    sendsize = sendsize + 16 + 15 + passwordLen;
                    memcpy(buffer + sendsize + 16*0, "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 16);
                    memcpy(buffer + sendsize + 16*1, "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 14);
                    memcpy(buffer + sendsize + 16*1 + 14, "fadylinux", 9); // local hostname
                    memcpy(buffer + sendsize + 16*1 + 14 + 9, "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x34\x48\xed", 16);
                    memcpy(buffer + sendsize + 16*1 + 14 + 9 + 16, "\x20\xd3\xab\x00\x00\x04\x00\x00\x00", 9);
                    sendsize = sendsize + 16 + 14 + 9 + 16 + 9;
                }

                if(send(this->_sockfd, buffer, sendsize, 0) == -1)
                    MaleniaException::show(LOGIN_RET_UNKNOWN);
                else
                {
                    memset(buffer, 0, sendsize);

                    recvsize = recv(this->_sockfd, buffer, SOCKET_BUFFER_SIZE, 0);
                    recvsize = recv(this->_sockfd, buffer, SOCKET_BUFFER_SIZE, 0);
                    if(recvsize > 300)
                    {
                        memset(buffer, 0, SOCKET_BUFFER_SIZE);

                        memcpy(buffer, "\x31\x31\x31\x31\x0c\x01\x00\x00\x04\x00\x00\x01\x1b\x09\x00\x00\x2e\x01\x00\x00\xfc\x00\x00\x00", 24);
                        memcpy(buffer + 24, username, usernameLen);

                        sendsize = 24 + 4 + 16*3 + 8; // some leading zeros
                        memcpy(buffer + sendsize, "\x00\x00\x00\x03queryBasicCfg", 17);
                        
                        sendsize = sendsize + 8 + 16*3 + 8 + 4; // some leading zeros
                        memcpy(buffer + sendsize, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<request version=\"1.0\" systemType=\"NVMS-9000\" clientType=\"WEB\" url=\"queryBasicCfg\"/>\n", 4 + 16*7 + 8);

                        sendsize = sendsize + 4 + 16*7 + 8;
                        if(send(this->_sockfd, buffer, sendsize, 0) == -1)
                            MaleniaException::show(LOGIN_RET_UNKNOWN);
                        else
                        {
                            memset(buffer, 0, sendsize);
                            recvsize = 0;

                            while(!bytes_contain(buffer, SOCKET_BUFFER_SIZE, "</response>"))
                                recvsize = recvsize + recv(this->_sockfd, buffer + recvsize, SOCKET_BUFFER_SIZE, 0);

                            for(i = 0; i < recvsize; i ++)
                                if(buffer[i] == '<')
                                    break;

                            result = doc.load_string(buffer + i);
                            if(result)
                            { 
                                root = doc.child("response").child("content");
                                strncpy_t(this->_serialNumber, MAX_SHORT_STR_LEN, root.child_value("sn"), MAX_SHORT_STR_LEN);
                                strncpy_t(this->_modelName, MAX_SHORT_STR_LEN, root.child_value("productModel"), MAX_SHORT_STR_LEN);

                                memset(buffer, 0, SOCKET_BUFFER_SIZE); 

                                memcpy(buffer, "\x31\x31\x31\x31\xb5\x01\x00\x00\x04\x00\x00\x01\x1b\x09\x00\x00\x31\x01\x00\x00\xa5\x01\x00\x00", 24);
                                memcpy(buffer + 24, username, usernameLen);

                                sendsize = 24 + 4 + 16*3 + 8; // some leading spaces
                                memcpy(buffer + sendsize, "\x00\x00\x00\x03queryDevList", 16);

                                sendsize = sendsize + 8 + 16*3 + 8 + 4; // some leading zeros
                                memcpy(buffer + sendsize, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<request version=\"1.0\" systemType=\"NVMS-9000\" clientType=\"WEB\" url=\"queryDevList\">\n\x09", 123);
                                memcpy(buffer + sendsize + 123, "<condition>\n\x09\x09<include1400/>\n\x09</condition>\n\x09<requireField>\n\x09\x09<name/>\n\x09\x09<chlIndex/>\n\x09\x09<chlType/>\n\x09\x09<ip/>\n\x09\x09", 106);
                                memcpy(buffer + sendsize + 123 + 106, "<port/>\n\x09\x09<userName/>\n\x09\x09<password/>\n\x09</requireField>\n</request>\n", 64);
                                
                                sendsize = sendsize + 123 + 106 + 64;
                                if(send(this->_sockfd, buffer, sendsize, 0) == -1)
                                    MaleniaException::show(LOGIN_RET_UNKNOWN);
                                else
                                {
                                    memset(buffer, 0, sendsize);
                                    
                                    recvsize = 0;
                                    while(!bytes_contain(buffer, SOCKET_BUFFER_SIZE, "</response>"))
                                        recvsize = recvsize + recv(this->_sockfd, buffer + recvsize, SOCKET_BUFFER_SIZE, 0);
                                    for(i = 0; i < recvsize; i ++)
                                        if(buffer[i] == '<')
                                            break;

                                    result = doc.load_string(buffer + i);
                                    if(result)
                                    { 
                                        root = doc.child("response").child("content");
                                        this->_channelsAmount = atoi(root.attribute("total").value());

                                        this->updateCredentials(username, password);

                                        for(i = 0; i < this->_channelsAmount; i++)
                                        {
                                            child = new SDK_Channel(this->_sockfd, i + 1, this);
                                            child->setText(0, QString("CH %1").arg(i + 1));
                                            this->_channels.push_back(child);
                                        }
                                        return 1;
                                    }
                                }
                            }
                        }
                    }
                    else
                        MaleniaException::show(LOGIN_RET_INVALID_CREDENTIALS);
                }
            }
        }
        
        return 0;
    }

    SDK_Channel::SDK_Channel(int sockfd, int chid, QTreeWidgetItem* parent) : IChannel(chid, parent)
    {
        this->_sockfd = sockfd;
    }

    SDK_Channel::~SDK_Channel()
    {
    }

    IStream* SDK_Channel::StartLiveView(QFrame* qframe)
    {
        return this->StartStream(new SDK_LiveStream(this->_sockfd, this->_chid, this), qframe);
    }

    IStream* SDK_Channel::StartPlayback(datetime from, datetime to, QFrame* qframe)
    {
        return this->StartStream(new SDK_PlaybackStream(this->_sockfd, this->_chid, from, to, this), qframe);
    }

    SDK_LiveStream::SDK_LiveStream(int sockfd, int chid, IChannel* channel, QWidget* parent) : IStream(channel, parent)
    {
        this->_sockfd = sockfd;
        this->_chid = chid;
    }

    SDK_LiveStream::~SDK_LiveStream()
    {
        this->Stop();
    }

    int SDK_LiveStream::Start(QFrame* qframe)
    {        
        return IStream::Start(qframe);
    }

    void SDK_LiveStream::Stop()
    {
        IStream::Stop();
    }

    SDK_PlaybackStream::SDK_PlaybackStream(int sockfd, int chid, datetime from, datetime to, IChannel* channel, QWidget* parent) : Playback_Stream(from, to, channel, parent)
    {
        this->_sockfd = sockfd;
        this->_chid = chid;
    }

    SDK_PlaybackStream::~SDK_PlaybackStream()
    {
        this->Stop();
    }

    int SDK_PlaybackStream::Start(QFrame* qframe)
    {
        return IStream::Start(qframe);
    }

    void SDK_PlaybackStream::Stop()
    {
        IStream::Stop();
    }
}

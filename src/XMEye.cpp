#include <sys/socket.h>
#include <arpa/inet.h>

#include "../headers/XMEye.h"
#include "../headers/external/CkHttp/CkHttpResponse.h"
#include "../headers/external/pugixml.hpp"
#include "../headers/external/md5.h"

#define NAT_SERVER_IP "52.58.4.30"
#define NAT_SERVER_PORT 8777
#define HASHED_PASSWORD_LEN 8 

namespace XMEye
{
    std::string FORMAT_RTSP_LIVE(std::string rtspUri, const char* username, const char* password, int camId, int port)
    {
        return rtspUri + std::to_string(port) + "/user=" + username + "&password=" + password + "&channel-" + std::to_string(camId) + "&stream=0.sdp";
    }

    std::string FORMAT_RTSP_PLAYBACK(std::string rtspUri, const char* username, const char* password, int camId, int port, datetime start, datetime stop)
    {
        return rtspUri + std::to_string(port) + "/user=" + username + "&password=" + password + "&channel-" + std::to_string(camId) + "&stream=0.sdp"; 
    }

    int parsePort(const char* input)
    {
        char pHex[5] = {0};
        // little endian
        strncpy_t(pHex, 5, int_to_hex((unsigned char)input[1]).c_str(), 2);
        strncpy_t(pHex + 2, 3, int_to_hex((unsigned char)input[0]).c_str(), 2);
        return (int)strtol(pHex, NULL, 16);
    }

    IDevice* getDeviceFromNAT(const char* sn)
    {
        char socketBuffer[SOCKET_BUFFER_SIZE] = {0};
        char ip[MAX_IP_LEN] = {0};
        int port = -1, sockfd = 0, recvsize = 0, snlen = 0;
        struct sockaddr_in addrinfo;
        socklen_t addrlen = sizeof(addrinfo);
        struct timeval tv;

        snlen = strnlen(sn, MAX_SHORT_STR_LEN);
        if(snlen == 0)
            return NULL;

        // Creating socket file descriptor 
        if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
            MaleniaException::show(LOGIN_RET_UNKNOWN, "Failed to create socket");
        else
        {
            tv.tv_sec = SOCKET_TIMEOUT_SECONDS;
            tv.tv_usec = 0;
            if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) 
                MaleniaException::show(LOGIN_RET_UNKNOWN, "Failed to set socket timeout");

            memset(&addrinfo, 0, sizeof(addrinfo)); 
            addrinfo.sin_family = AF_INET; 
            addrinfo.sin_port = htons(NAT_SERVER_PORT);
            addrinfo.sin_addr.s_addr = inet_addr(NAT_SERVER_IP);

            memcpy(socketBuffer, "\x15\x20\x00\xb0", 4);
            memcpy(socketBuffer + 4, sn, snlen);
            if (sendto(sockfd, socketBuffer, 4 + snlen, 0, (struct sockaddr*)&addrinfo, addrlen) < 0)
                MaleniaException::show(LOGIN_RET_TIMEOUT, "Failed to send packet #1 to NAT Server");
            else
            {
                memset(socketBuffer, 0, 4);
                recvsize = recvfrom(sockfd, socketBuffer, SOCKET_BUFFER_SIZE, 0, 
                                    (struct sockaddr*)&addrinfo, &addrlen);
                if(recvsize <= 100)
                    MaleniaException::show(ERR_INVALID_SN, "Failed to receive packet #1 from NAT Server");
                else
                {
                    memcpy(ip, socketBuffer + 108, MAX_IP_LEN);
                    port = parsePort(socketBuffer + 108 + 20);
                    addrinfo.sin_port = htons(port);
                    addrinfo.sin_addr.s_addr = inet_addr(ip);
                    
                    memset(socketBuffer, 0, recvsize);
                    memcpy(socketBuffer, "\x12\x20\xe8\x03", 4);
                    if (sendto(sockfd, socketBuffer, 4, 0, (struct sockaddr*)&addrinfo, addrlen) < 0)
                        MaleniaException::show(LOGIN_RET_TIMEOUT, "Failed to send packet #2 to NAT Server");
                    else
                    {
                        memset(socketBuffer, 0, 4);
                        recvsize = recvfrom(sockfd, socketBuffer, SOCKET_BUFFER_SIZE, 0, 
                                            (struct sockaddr*)&addrinfo, &addrlen);
                        if(recvsize <= 0)
                            MaleniaException::show(LOGIN_RET_TIMEOUT, "Failed to receive packet #2 from NAT Server");
                        else
                        {
                            memcpy(socketBuffer + 208, socketBuffer + 4, recvsize - 4 - 2);
                            memset(socketBuffer, 0, recvsize);
                            memcpy(socketBuffer, "\x12\x20\xec\x03\xe9\x03", 6);
                            memcpy(socketBuffer + 8, "!xmnatuuid-IEClient-5352004118467", 33);
                            memcpy(socketBuffer + 108, sn, snlen);
                            memcpy(socketBuffer + 208 + recvsize - 4 -2, "\x3f\xf4""10.0.2.15", 11);
                            memcpy(socketBuffer + 352, "\x04\x19\x00\x00\x00\x00\x00\x00\x01", 9);

                            if (sendto(sockfd, socketBuffer, 364, 0, (struct sockaddr*)&addrinfo, addrlen) < 0)
                                MaleniaException::show(LOGIN_RET_TIMEOUT, "Failed to send packet #3 to NAT Server");
                            else
                            {
                                memset(socketBuffer, 0, 364);
                                recvsize = recvfrom(sockfd, socketBuffer, SOCKET_BUFFER_SIZE, 0, 
                                            (struct sockaddr*)&addrinfo, &addrlen);
                                if(recvsize <= 0)
                                    MaleniaException::show(LOGIN_RET_TIMEOUT, "Failed to receive packet #3 from NAT Server");
                                else
                                {
                                    memset(socketBuffer, 0, recvsize);
                                    recvsize = recvfrom(sockfd, socketBuffer, SOCKET_BUFFER_SIZE, 0, 
                                                        (struct sockaddr*)&addrinfo, &addrlen);
                                    if(recvsize <= 0)
                                        MaleniaException::show(LOGIN_RET_TIMEOUT, "Failed to receive packet #4 from NAT Server");
                                    else
                                    {
                                        memset(ip, 0, MAX_IP_LEN);  
                                        memcpy(ip, socketBuffer + 16*13, MAX_IP_LEN);
                                        port = parsePort(socketBuffer + 16*14);
                                        addrinfo.sin_port = htons(port);
                                        addrinfo.sin_addr.s_addr = inet_addr(ip);

                                        return new P2P_Device(sockfd, *((struct sockaddr*)&addrinfo), sn, ip, port);
                                    }
                                }
                            }
                        }
                    }
                }
            }

        close(sockfd); // if something failed in the meantime
        }
    
        return NULL;
    }

    HTTP_Device::HTTP_Device(const char* ip, const int port) : HTTPDevice(ip, port)
    {
    }

    HTTP_Device::~HTTP_Device()
    {
    }

    int HTTP_Device::login(const char* username, const char* password)
    {
        return 1;
    }

    P2P_Device::P2P_Device(int sockfd, struct sockaddr addrinfo, const char* serialNumber,
                           const char* ip, const int port) : P2PDevice(sockfd, addrinfo, serialNumber, ip, port)
    {
    }

    P2P_Device::~P2P_Device()
    {
    }

    int P2P_Device::login(const char* username, const char* password)
    {
        char socketBuffer[SOCKET_BUFFER_SIZE] = {0};
        std::string passwordHashed;
        int recvsize = 0, usernameLen = strnlen(username, MAX_SHORT_STR_LEN), i = 0, hashTmp = 0;
        JSONB jsonb;
        socklen_t addrlen = sizeof(this->_addrinfo);
        MD5::uint1* hex = MD5(password).getDigest();
        IChannel* child = NULL;

        for(i = 0; i < HASHED_PASSWORD_LEN; i++)
        {
            hashTmp = (hex[2*i] + hex[2*i+1]) % 0x3e;
            if(hashTmp > 9)
            {
                if(hashTmp > 35)
                    hashTmp += 61;
                else
                    hashTmp += 55;
            }
            else
                hashTmp += 0x30;

            passwordHashed += (char)hashTmp;
        }

        memcpy(socketBuffer, "\x12\x20\xd0\x07\xe9\x03", 6);
        memcpy(socketBuffer + 8, "!xmnatuuid-IEClient-5352004118467", 33);
        if (sendto(this->_sockfd, socketBuffer, 16*7, 0, &this->_addrinfo, addrlen) < 0)
            MaleniaException::show(LOGIN_RET_TIMEOUT, "Failed to send packet #1 to Client");
        else
        {
            memset(socketBuffer, 0, 6 + 33);
            recvsize = recvfrom(this->_sockfd, socketBuffer, SOCKET_BUFFER_SIZE, 0, &this->_addrinfo, &addrlen);
            if(recvsize <= 0)
                MaleniaException::show(LOGIN_RET_TIMEOUT, "Failed to receive packet #1 from Client");
            else
            {
                memset(socketBuffer, 0, recvsize);

                memcpy(socketBuffer, "\x12\x20\xd1\x07\xe9\x03", 6);
                memcpy(socketBuffer + 8, "!xmnatuuid-IEClient-5352004118467", 33);
                if (sendto(this->_sockfd, socketBuffer, 16*7, 0, &this->_addrinfo, addrlen) < 0)
                    MaleniaException::show(LOGIN_RET_TIMEOUT, "Failed to send packet #2 to Client");
                else
                {
                    memset(socketBuffer, 0, 33 + 6);
                    memcpy(socketBuffer + 16*0, "\x12\x20\xd0\x07\xe9\x03\x00\x00\x21\x78\x6d\x6e\x61\x74\x75\x75", 16);
                    memcpy(socketBuffer + 16*1, "\x69\x64\x2d\x49\x45\x43\x6c\x69\x65\x6e\x74\x2d\x35\x33\x35\x32", 16);
                    memcpy(socketBuffer + 16*2, "\x30\x30\x34\x31\x31\x38\x34\x36\x37", 9);
                    memcpy(socketBuffer + 16*6 + 12, "\x01", 1);
                    if (sendto(this->_sockfd, socketBuffer, 16*7, 0, &this->_addrinfo, addrlen) < 0)
                        MaleniaException::show(LOGIN_RET_TIMEOUT, "Failed to send packet #3 to Client");
                    else
                    {
                        memset(socketBuffer, 0, 16*7);
                        recvsize = recvfrom(this->_sockfd, socketBuffer, SOCKET_BUFFER_SIZE, 0, &this->_addrinfo, &addrlen);
                        if(recvsize <= 0)
                            MaleniaException::show(LOGIN_RET_TIMEOUT, "Failed to receive packet #2 from Client");
                        else
                        {
                            memset(socketBuffer, 0, recvsize);
                            recvsize = recvfrom(this->_sockfd, socketBuffer, SOCKET_BUFFER_SIZE, 0, &this->_addrinfo, &addrlen);
                            if(recvsize <= 0)
                                MaleniaException::show(LOGIN_RET_TIMEOUT, "Failed to receive packet #3 from Client");
                            else
                            {
                                memset(socketBuffer, 0, recvsize);
                                memcpy(socketBuffer + 16*0, "\x12\x20\xd0\x07\xe9\x03\x00\x00\x21\x78\x6d\x6e\x61\x74\x75\x75", 16);
                                memcpy(socketBuffer + 16*1, "\x69\x64\x2d\x49\x45\x43\x6c\x69\x65\x6e\x74\x2d\x35\x33\x35\x32", 16);
                                memcpy(socketBuffer + 16*2, "\x30\x30\x34\x31\x31\x38\x34\x36\x37", 9);
                                memcpy(socketBuffer + 16*6 + 12, "\x01", 1);
                                if (sendto(this->_sockfd, socketBuffer, 16*7, 0, &this->_addrinfo, addrlen) < 0)
                                    MaleniaException::show(LOGIN_RET_TIMEOUT, "Failed to send packet #4 to Client");
                                else
                                {
                                    memset(socketBuffer, 0, 16*7);
                                    recvsize = recvfrom(this->_sockfd, socketBuffer, SOCKET_BUFFER_SIZE, 0, &this->_addrinfo, &addrlen);
                                    if(recvsize <= 0)
                                        MaleniaException::show(LOGIN_RET_TIMEOUT, "Failed to receive packet #4 from Client");
                                    else
                                    {
                                        memset(socketBuffer, 0, recvsize);
                                        recvsize = recvfrom(this->_sockfd, socketBuffer, SOCKET_BUFFER_SIZE, 0, &this->_addrinfo, &addrlen);
                                        if(recvsize <= 0)
                                            MaleniaException::show(LOGIN_RET_TIMEOUT, "Failed to receive packet #5 from Client");
                                        else
                                        {
                                            memset(socketBuffer, 0, recvsize);
                                            memcpy(socketBuffer + 16*0, "\x12\x20\xd3\x07\x00\x00\xdf\x00\x00\x00\x00\x00\x58\x4d\x49\x50", 16);
                                            memcpy(socketBuffer + 16*1, "\x00\x00\x00\x00\x01\x0e\x00\x00\xff\x00\x00\x00\x9f\x86\x01\x00", 16);
                                            memcpy(socketBuffer + 16*2, "\x00\x00\x00\x00\x63\x00\x85\x05\xbf", 9);
                                            memcpy(socketBuffer + 44, "{ \"Name\": \"OPMonitor\",  \"OPMonitor\" : { \"Action\" : \"Claim\", \"Parameter\" : { \"Channel\" : 0, \"CombinMode\" : \"CONNECT_ALL\", \"StreamType\" : \"Main\", \"TransMode\" : \"TCP\" } }, \"SessionID\" : \"0x1\" }\x0a", 191);
                                            if (sendto(this->_sockfd, socketBuffer, 235, 0, &this->_addrinfo, addrlen) < 0)
                                                MaleniaException::show(LOGIN_RET_TIMEOUT, "Failed to send packet #5 to Client");
                                            else
                                            {
                                                memset(socketBuffer, 0, 235);
                                                memcpy(socketBuffer + 16*0, "\x12\x20\xd1\x07\xe9\x03\x00\x00\x21\x78\x6d\x6e\x61\x74\x75\x75", 16);
                                                memcpy(socketBuffer + 16*1, "\x69\x64\x2d\x49\x45\x43\x6c\x69\x65\x6e\x74\x2d\x35\x33\x35\x32", 16);
                                                memcpy(socketBuffer + 16*2, "\x30\x30\x34\x31\x31\x38\x34\x36\x37", 9);
                                                memcpy(socketBuffer + 16*6 + 12, "\x01", 1);
                                                if (sendto(this->_sockfd, socketBuffer, 16*7, 0, &this->_addrinfo, addrlen) < 0)
                                                    MaleniaException::show(LOGIN_RET_TIMEOUT, "Failed to send packet #6 to Client");
                                                {
                                                    memset(socketBuffer, 0, 16*7);
                                                    recvsize = recvfrom(this->_sockfd, socketBuffer, SOCKET_BUFFER_SIZE, 0, &this->_addrinfo, &addrlen);
                                                    if(recvsize <= 0)
                                                        MaleniaException::show(LOGIN_RET_TIMEOUT, "Failed to receive packet #6 from Client");
                                                    else
                                                    {
                                                        memset(socketBuffer, 0, recvsize);
                                                        recvsize = recvfrom(this->_sockfd, socketBuffer, SOCKET_BUFFER_SIZE, 0, &this->_addrinfo, &addrlen);
                                                        if(recvsize <= 0)
                                                            MaleniaException::show(LOGIN_RET_TIMEOUT, "Failed to receive packet #7 from Client");
                                                        else
                                                        {
                                                            memset(socketBuffer, 0, recvsize);
                                                            memcpy(socketBuffer + 16*0, "\x12\x20\xd3\x07\x00\x00\x0c\x00\x00\x00\x00\x00\x58\x4d\x49\x50", 16);
                                                            memcpy(socketBuffer + 16*1, "\x00\x00\x00\x00\x01\x01", 6);
                                                            if (sendto(this->_sockfd, socketBuffer, 24, 0, &this->_addrinfo, addrlen) < 0)
                                                                MaleniaException::show(LOGIN_RET_TIMEOUT, "Failed to send packet #7 to Client");
                                                            else
                                                            {
                                                                memset(socketBuffer, 0, 24);
                                                                memcpy(socketBuffer + 16*0, "\x12\x20\xd3\x07\x00\x00\x9b\x00\x00\x00\x00\x00\x58\x4d\x49\x50", 16);
                                                                memcpy(socketBuffer + 16*1, "\x01\x00\x00\x00\x01\x0e\x00\x00\xff\x00\x00\x00\x00\x00\x00\x00", 16);
                                                                memcpy(socketBuffer + 16*2, "\x00\x00\x00\x00\x00\x00\xe8\x03\x7b\x00\x00\x00", 12);
                                                                memcpy(socketBuffer + 16*2 + 12, "{ \"CommunicateKey\" : \"\", \"EncryptType\" : \"MD5\", \"LoginType\" : \"DVRIP-Web\", \"PassWord\" : \"", 89);
                                                                memcpy(socketBuffer + 16*2 + 12 + 89, passwordHashed.c_str(), HASHED_PASSWORD_LEN);
                                                                memcpy(socketBuffer + 16*2 + 12 + 89 + HASHED_PASSWORD_LEN, "\", \"UserName\" : \"", 17);
                                                                memcpy(socketBuffer + 16*2 + 12 + 89 + HASHED_PASSWORD_LEN + 17, username, usernameLen);
                                                                memcpy(socketBuffer + 16*2 + 12 + 89 + HASHED_PASSWORD_LEN + 17 + usernameLen, "\" }\x0a", 4);
                                                                if (sendto(this->_sockfd, socketBuffer, 16*2 + 12 + 89 + HASHED_PASSWORD_LEN + 17 + usernameLen + 4, 0, &this->_addrinfo, addrlen) < 0)
                                                                    MaleniaException::show(LOGIN_RET_TIMEOUT, "Failed to send packet #8 to Client");
                                                                else
                                                                {
                                                                    memset(socketBuffer, 0, 16*2 + 12 + 89 + HASHED_PASSWORD_LEN + 17 + usernameLen + 4);
                                                                    recvsize = recvfrom(this->_sockfd, socketBuffer, SOCKET_BUFFER_SIZE, 0, &this->_addrinfo, &addrlen);
                                                                    if(recvsize <= 0)
                                                                        MaleniaException::show(LOGIN_RET_TIMEOUT, "Failed to receive packet #7 from Client");
                                                                    else
                                                                    {
                                                                        memset(socketBuffer, 0, recvsize);
                                                                        recvsize = recvfrom(this->_sockfd, socketBuffer, SOCKET_BUFFER_SIZE, 0, &this->_addrinfo, &addrlen);
                                                                        if(recvsize <= 0)
                                                                            MaleniaException::show(LOGIN_RET_TIMEOUT, "Failed to receive packet #8 from Client");
                                                                        else
                                                                        {
                                                                            socketBuffer[recvsize - 2] = 0;
                                                                            jsonb.parse(socketBuffer + 44);
                                                                            if(strncmp(jsonb.getValue("Ret"), "100", MAX_SHORT_STR_LEN) == 0)
                                                                            {
                                                                                this->_channelsAmount = atoi(jsonb.getValue("ChannelNum"));
                                                                                strncpy_t(this->_modelName, MAX_SHORT_STR_LEN, jsonb.getValue("DeviceType"), MAX_SHORT_STR_LEN);

                                                                                this->updateCredentials(username, password);

                                                                                for(i = 0; i < this->_channelsAmount; i++)
                                                                                {
                                                                                    child = new P2P_Channel(this->_sockfd, i + 1, this);
                                                                                    child->setText(0, QString("CH %1").arg(i + 1));
                                                                                    this->_channels.push_back(child);
                                                                                }
                                                                                return 1;
                                                                            }
                                                                            else
                                                                                MaleniaException::show(LOGIN_RET_INVALID_CREDENTIALS);
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        return 0;
    }

    P2P_Channel::P2P_Channel(int sockfd, int chid, QTreeWidgetItem* parent) : IChannel(chid, parent)
    {
        this->_sockfd = sockfd;
    }

    P2P_Channel::~P2P_Channel()
    {
    }

    IStream* P2P_Channel::StartLiveView(QFrame* qframe)
    {
        return this->StartStream(new P2P_LiveStream(this->_sockfd, this->_chid, this), qframe);
    }

    IStream* P2P_Channel::StartPlayback(datetime from, datetime to, QFrame* qframe)
    {
        return this->StartStream(new P2P_PlaybackStream(this->_sockfd, this->_chid, from, to, this), qframe);
    }

    P2P_LiveStream::P2P_LiveStream(int sockfd, int chid, IChannel* channel, QWidget* parent) : IStream(channel, parent)
    {
        this->_sockfd = sockfd;
        this->_chid = chid;
    }

    P2P_LiveStream::~P2P_LiveStream()
    {
        this->Stop();
    }

    int P2P_LiveStream::Start(QFrame* qframe)
    {        
        return IStream::Start(qframe);
    }

    void P2P_LiveStream::Stop()
    {
        IStream::Stop();
    }

    P2P_PlaybackStream::P2P_PlaybackStream(int sockfd, int chid, datetime from, datetime to, IChannel* channel, QWidget* parent) : Playback_Stream(from, to, channel, parent)
    {
        this->_sockfd = sockfd;
        this->_chid = chid;
    }

    P2P_PlaybackStream::~P2P_PlaybackStream()
    {
        this->Stop();
    }

    int P2P_PlaybackStream::Start(QFrame* qframe)
    {
        return IStream::Start(qframe);
    }

    void P2P_PlaybackStream::Stop()
    {
        IStream::Stop();
    }

}

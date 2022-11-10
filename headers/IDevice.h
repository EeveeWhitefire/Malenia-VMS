#pragma once
#ifndef IDEVICE_H
#define IDEVICE_H

#include "../headers/utils.hpp"
#include "../headers/external/CkHttp/CkHttp.h"
#include "../headers/IChannel.h"

#include <netinet/in.h>

#include <QTreeWidgetItem>

enum SDK_LOGIN_MODE {LOGIN_MODE_SDK, LOGIN_MODE_HTTP};

class IDevice : public QTreeWidgetItem
{
    public:
        IDevice(const char*, int);
        virtual ~IDevice();
        virtual int login(const char*, const char*) = 0;
        virtual void updateCredentials(const char*, const char*);
        const char* getIP();
        int getPort();
        int getChannelsAmount();
        IChannel* getFreeChannel();
        const char* getSerialNumber();
        const char* getModelName();
        datetime getDatetimeOffset();
    protected:
        char _ip[MAX_IP_LEN] = {0};
        int _port;
        char _serialNumber[MAX_STR_LEN] = {0};
        char _modelName[MAX_SHORT_STR_LEN] = {0};
        char _username[MAX_SHORT_STR_LEN] = {0};
        char _password[MAX_SHORT_STR_LEN] = {0};
        int _channelsAmount = 0;
        datetime _dtOffset;
        std::vector<IChannel*> _channels;
};

class HTTPDevice : public IDevice
{
    public:
        HTTPDevice(const char*, int);
        virtual ~HTTPDevice() = 0;
        virtual void updateCredentials(const char*, const char*);
    protected:
        CkHttp _http;
        std::string _uriBegin;
        std::string _rtspUriBegin;
};

class SDKDevice : public IDevice
{
    public:
        SDKDevice(SDK_LOGIN_MODE, const char*, int);
        virtual ~SDKDevice() = 0;
    protected:
        SDK_LOGIN_MODE _loginMode;
};

class P2PDevice : public IDevice
{
    public:
        P2PDevice(int, struct sockaddr, const char*, const char*, const int);
        virtual ~P2PDevice() = 0;
    protected:
        int _sockfd;
        struct sockaddr _addrinfo;
};

int isValidIP(const char*);
#endif

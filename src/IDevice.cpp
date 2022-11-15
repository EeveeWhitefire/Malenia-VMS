#include <QString>

#include "../headers/IDevice.h"
#include "../headers/utils.hpp"
#include "../headers/colors.h"
#include "../headers/external/CkHttp/CkSettings.h"


int isValidIP(const char* ip)
{
    size_t i = 0;
    int dotC = 0;
    int nonDigitDetected = 0;
    for(i = 0; i < strnlen(ip, MAX_IP_LEN); i++)
        if(ip[i] == '.')
            dotC++;
        else if(!isdigit(ip[i]))
        {
            nonDigitDetected = 1;
            break;
        }

    return dotC == 3 && !nonDigitDetected;
}

IDevice::IDevice(const char* ip, int port) : QTreeWidgetItem()
{
    strncpy_t(this->_ip, MAX_IP_LEN, ip, MAX_IP_LEN);
    this->_port = port;

    this->setText(0, ip);
    this->setText(1, QString::number(port));
    this->setForeground(0, QBrush(QColor(234, 234, 234)));
    this->setForeground(1, QBrush(QColor(234, 234, 234)));
}

IDevice::~IDevice()
{
    for(IChannel* channel: this->_channels)
        delete channel;
}

const char* IDevice::getIP()
{
    return this->_ip;
}

int IDevice::getPort()
{
    return this->_port;
}

const char* IDevice::getSerialNumber()
{
    return this->_serialNumber;
}

const char* IDevice::getModelName()
{
    return this->_modelName;
}

int IDevice::getChannelsAmount()
{
    return this->_channelsAmount;
}

IChannel* IDevice::getFreeChannel()
{
    for(IChannel* ch: this->_channels)
        if(!ch->isLiveStreaming())
            return ch;

    return NULL;
}

void IDevice::updateCredentials(const char* username, const char* password)
{
    strncpy_t(this->_username, MAX_SHORT_STR_LEN, username, MAX_SHORT_STR_LEN);
    strncpy_t(this->_password, MAX_SHORT_STR_LEN, password, MAX_SHORT_STR_LEN);
}

datetime IDevice::getDatetimeOffset()
{
    return this->_dtOffset;
}

HTTPDevice::HTTPDevice(const char* ip, const int port) : IDevice(ip, port)
{
    this->_uriBegin = "http://" + std::string(ip) + ":" + std::to_string(port);
}

HTTPDevice::~HTTPDevice()
{
    CkSettings::cleanupMemory();
}

void HTTPDevice::updateCredentials(const char* username, const char* password)
{
    IDevice::updateCredentials(username, password);
    this->_rtspUriBegin = "rtsp://" + std::string(this->_username) + ":" + std::string(this->_password) + "@" + std::string(this->_ip) + ":554";
}

SDKDevice::SDKDevice(SDK_LOGIN_MODE loginMode, const char* ip, int port) : IDevice(ip, port)
{
    this->_loginMode = loginMode;
}

SDKDevice::~SDKDevice()
{

}

P2PDevice::P2PDevice(int sockfd, struct sockaddr addrinfo, const char* serialNumber, const char* ip, const int port) : IDevice(ip, port)
{
    _sockfd = sockfd;
    memcpy(&_addrinfo, &addrinfo, sizeof(addrinfo));
    strncpy_t(this->_serialNumber, MAX_SHORT_STR_LEN, serialNumber, MAX_SHORT_STR_LEN);
}

P2PDevice::~P2PDevice()
{
    close(_sockfd);
}

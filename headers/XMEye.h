#pragma once

#include "../headers/IDevice.h"

namespace XMEye
{
    class HTTP_Device : public HTTPDevice
    {
        public:
            HTTP_Device(const char*, const int);
            virtual ~HTTP_Device();
            virtual int login(const char*, const char*);
    };

    class P2P_Device : public P2PDevice
    {
        public:
            P2P_Device(int, struct sockaddr, const char*, const char*, const int);
            virtual ~P2P_Device();
            virtual int login(const char*, const char*);
    };

    class P2P_Channel : public IChannel
    {
        public:
            P2P_Channel(int, int, QTreeWidgetItem*);
            virtual ~P2P_Channel();
            virtual IStream* StartLiveView(QFrame*);
            virtual IStream* StartPlayback(datetime, datetime, QFrame*);
        private:
            int _sockfd = 0;
    };

    class P2P_LiveStream : public IStream
    {
        public:
            P2P_LiveStream(int, int, IChannel*, QWidget* = NULL);
            virtual ~P2P_LiveStream();
            virtual int Start(QFrame*);
            virtual void Stop();
        private:
            int _sockfd = 0;
            int _chid = 0;
    };

    class P2P_PlaybackStream : public Playback_Stream
    {
        public:
            P2P_PlaybackStream(int, int, datetime, datetime, IChannel*, QWidget* = NULL);
            virtual ~P2P_PlaybackStream();
            virtual int Start(QFrame*);
            virtual void Stop();
        private:
            int _sockfd = 0;
            int _chid = 0;
    };

    IDevice* getDeviceFromNAT(const char*);
    std::string FORMAT_RTSP_LIVE(const std::string, const char*, const char*, int, int);
    std::string FORMAT_RTSP_PLAYBACK(const std::string, const char*, const char*, int, int, datetime, datetime);

    int parsePort(const char*);
}

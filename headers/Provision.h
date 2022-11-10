#pragma once

#include "../headers/IDevice.h"

namespace Provision
{
    class HTTP_Device : public HTTPDevice
    {
        public:
            HTTP_Device(const char*, const int);
            virtual ~HTTP_Device();
            virtual int login(const char*, const char*);
    };

    class HTTP_Channel : public RTSPChannel
    {
        public:
            HTTP_Channel(std::string, int, QTreeWidgetItem* = NULL);
            ~HTTP_Channel();
            virtual IStream* StartLiveView(QFrame*);
            virtual IStream* StartPlayback(datetime, datetime, QFrame*);
    };

    class SDK_Device : public IDevice
    {
        public:
            SDK_Device(const char*, const int);
            virtual ~SDK_Device();
            virtual int login(const char*, const char*);
        private:
            int _sockfd = 0;
    };

    class SDK_Channel : public IChannel
    {
        public:
            SDK_Channel(int, int, QTreeWidgetItem*);
            virtual ~SDK_Channel();
            virtual IStream* StartLiveView(QFrame*);
            virtual IStream* StartPlayback(datetime, datetime, QFrame*);
        private:
            int _sockfd = 0;
    };

    class SDK_LiveStream : public IStream
    {
        public:
            SDK_LiveStream(int, int, IChannel*, QWidget* = NULL);
            virtual ~SDK_LiveStream();
            virtual int Start(QFrame*);
            virtual void Stop();
        private:
            int _sockfd = 0;
            int _chid = 0;
    };

    class SDK_PlaybackStream : public Playback_Stream
    {
        public:
            SDK_PlaybackStream(int, int, datetime, datetime, IChannel*, QWidget* = NULL);
            virtual ~SDK_PlaybackStream();
            virtual int Start(QFrame*);
            virtual void Stop();
        private:
            int _sockfd = 0;
            int _chid = 0;
    };

    std::string FORMAT_RTSP_LIVE(const std::string, int, int);
    std::string FORMAT_RTSP_PLAYBACK(const std::string, int, int, datetime, datetime);
}

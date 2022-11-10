#pragma once

#include "../headers/IDevice.h"
#include "../headers/external/HCNetSDK.h"

namespace Hikvision
{
    class SDK_Device : public SDKDevice
    {
        public:
            SDK_Device(SDK_LOGIN_MODE, const char*, const int);
            virtual ~SDK_Device();
            virtual int login(const char*, const char*);
        private:
            int _userId = 0;
            std::map<int, int> _chIds;
    };

    class SDK_Channel : public IChannel
    {
        public:
            SDK_Channel(int, int, QTreeWidgetItem* = NULL);
            ~SDK_Channel();
            virtual IStream* StartLiveView(QFrame*);
            virtual IStream* StartPlayback(datetime, datetime, QFrame*);
        private:
            int _userId = 0;
    };

    class SDK_LiveStream : public IStream
    {
        public:
            SDK_LiveStream(int, int, IChannel*, QWidget* = NULL);
            virtual ~SDK_LiveStream();
            virtual int Start(QFrame*);
            virtual void Stop();
            virtual int Replay();
        private:
            int _userId = 0;
            int _handle = HANDLE_RESET;
            int _chid = 0;
    };

    class SDK_PlaybackStream : public Playback_Stream
    {
        public:
            SDK_PlaybackStream(int, int, datetime, datetime, IChannel*, QWidget* = NULL);
            virtual ~SDK_PlaybackStream();
            virtual int Start(QFrame*);
            virtual void TogglePause();
            virtual void Stop();
            virtual int Replay();
        private:
            int _userId = 0;
            int _handle = HANDLE_RESET;
            int _chid = 0;
    };
}

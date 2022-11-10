#pragma once

#include "../headers/IDevice.h"
#include "../headers/external/dhnetsdk.h"

namespace Dahua
{
    class SDK_Device : public SDKDevice
    {
        public:
            SDK_Device(SDK_LOGIN_MODE, const char*, const int);
            virtual ~SDK_Device();
            virtual int login(const char*, const char*);
        private:
            LLONG _userId;
    };

    class SDK_Channel : public IChannel
    {
        public:
            SDK_Channel(LLONG, int, QTreeWidgetItem* = NULL);
            ~SDK_Channel();
            virtual IStream* StartLiveView(QFrame*);
            virtual IStream* StartPlayback(datetime, datetime, QFrame*);
        private:
            LLONG _userId = 0;
    };

    class SDK_LiveStream : public IStream
    {
        public:
            SDK_LiveStream(LLONG, int, IChannel*, QWidget* = NULL);
            virtual ~SDK_LiveStream();
            virtual int Start(QFrame*);
            virtual void Stop();
            virtual int Replay();
        private:
            LLONG _userId = 0;
            LLONG _handle = HANDLE_RESET;
            int _chid = 0;
            int _streamingPort = 0;
    };

    class SDK_PlaybackStream : public Playback_Stream
    {
        public:
            SDK_PlaybackStream(LLONG, int, datetime, datetime, IChannel*, QWidget* = NULL);
            virtual ~SDK_PlaybackStream();
            virtual int Start(QFrame*);
            virtual void TogglePause();
            virtual void Stop();
            virtual int Replay();
        private:
            LLONG _userId = 0;
            LLONG _handle = HANDLE_RESET;
            int _chid = 0;
            int _streamingPort = 0;
    };
}

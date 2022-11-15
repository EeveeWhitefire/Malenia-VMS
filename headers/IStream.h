#pragma once
#ifndef ISTREAM_H
#define ISTREAM_H

#include <QFrame>
#include <QMediaPlayer>
#include <QVideoWidget>

class IChannel;
#include "../headers/utils.hpp"
#include "../headers/IChannel.h"

#define HANDLE_RESET -1

class IStream : public QWidget
{
    public:
        IStream(IChannel*, QWidget* parent = NULL);
        virtual ~IStream();
        virtual int Start(QFrame*) = 0;
        virtual void Stop();
        virtual int Replay();
        QFrame* getLastFrame();
        IChannel* getChannel();
        int isPlaying();
    protected:
        QFrame* _lastFrame = NULL;
        IChannel* _channel = NULL;
        int _isPlaying = 0;
};

class RTSP_Stream : public IStream
{
    public:
            RTSP_Stream(std::string, IChannel*, QWidget* parent = NULL);
            virtual ~RTSP_Stream();
            virtual int Start(QFrame*);
            virtual void Stop();
    protected:
            std::string _rtspUri;
            QMediaPlayer _mediaPlayer;
            QVideoWidget* _mediaWidget = NULL;
};

class Playback_Stream : public IStream
{
    public:
        Playback_Stream(datetime, datetime, IChannel*, QWidget* parent = NULL);
        ~Playback_Stream();
        virtual void TogglePause();
    protected:
        datetime _from;
        datetime _to;
};
#endif

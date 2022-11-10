#pragma once
#ifndef ICHANNEL_H
#define ICHANNEL_H

#include <QTreeWidgetItem>
#include <QFrame>

class IStream;
#include "../headers/IStream.h"

class IChannel : public QTreeWidgetItem
{
    public:
        IChannel(int, QTreeWidgetItem* parent = NULL);
        virtual ~IChannel();

        virtual IStream* StartLiveView(QFrame*) = 0;
        virtual IStream* StartPlayback(datetime, datetime, QFrame*) = 0;
        IStream* StartStream(IStream*, QFrame*);
        void StopStream(IStream*);
        void StopLiveStream();
        void StopAllStreams();
        int isLiveStreaming();
        IStream* getLiveStream();
        datetime getDatetimeOffset();
    protected:
        IStream* _liveStream = NULL;
        std::vector<IStream*> _streams;
        int _chid = 0;
        datetime _dtOffset;
};

class RTSPChannel : public IChannel
{
    public:
        RTSPChannel(std::string, int, QTreeWidgetItem* parent = NULL);
        virtual ~RTSPChannel();
    protected:
        std::string _rtspUriBegin; 
};
#endif

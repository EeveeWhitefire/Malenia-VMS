#include "../headers/IDevice.h"
#include "../headers/IChannel.h"

IChannel::IChannel(int chid, QTreeWidgetItem* parent) : QTreeWidgetItem(parent)
{
    this->_chid = chid;
    this->_dtOffset = ((IDevice*)parent)->getDatetimeOffset();
}

IChannel::~IChannel()
{
    this->StopAllStreams();
}

IStream* IChannel::StartStream(IStream* stream, QFrame* qframe)
{    
    if(stream->Start(qframe)) // started successfully
    {
        this->_streams.push_back(stream);
        return stream;
    }

    delete stream;
    return NULL;
}

void IChannel::StopStream(IStream* stream)
{
    for(auto it = this->_streams.begin(); it != this->_streams.end(); ++it)
    {
        if(*it == stream) // found the stream
        {
            if(stream == this->_liveStream)
                this->_liveStream = NULL;

            this->_streams.erase(it);
            break;
        }
    }

    delete stream;
}

void IChannel::StopLiveStream()
{
    if(this->_liveStream != NULL)
        this->StopStream(this->_liveStream);
}

void IChannel::StopAllStreams()
{
    for(IStream* stream: this->_streams)
        delete stream;

    this->_liveStream = NULL;
    this->_streams.clear();
}

int IChannel::isLiveStreaming()
{
    return this->_liveStream != NULL;
}

IStream* IChannel::getLiveStream()
{
    return this->_liveStream;
}

datetime IChannel::getDatetimeOffset()
{
    return this->_dtOffset;
}

RTSPChannel::RTSPChannel(std::string rtspUriBegin, int chid, QTreeWidgetItem* parent) : IChannel(chid, parent)
{
    this->_rtspUriBegin = rtspUriBegin;
}

RTSPChannel::~RTSPChannel()
{

}

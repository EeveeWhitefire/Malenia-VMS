#include "../headers/IStream.h"

IStream::IStream(IChannel* channel, QWidget* parent) : QWidget(parent)
{
    this->_channel = channel;
}

IStream::~IStream()
{
}

int IStream::Start(QFrame* qframe)
{
    this->_lastFrame = qframe;
    this->_isPlaying = 1;
    return 1;
}

void IStream::Stop()
{
    if(this->_lastFrame != NULL)
    {
        // refreshing it
        this->_lastFrame->hide();
        this->_lastFrame->show();
    }

    this->_lastFrame = NULL;
    this->_isPlaying = 0;
}

int IStream::Replay()
{
    QFrame* lastFrame = this->_lastFrame;
    this->Stop();
    return this->Start(lastFrame);
}

QFrame* IStream::getLastFrame()
{
    return this->_lastFrame;
}

IChannel* IStream::getChannel()
{
    return this->_channel;
}

int IStream::isPlaying()
{
    return this->_isPlaying;
}

RTSP_Stream::RTSP_Stream(std::string rtspUri, IChannel* channel, QWidget* parent) : IStream(channel, parent)
{
    this->_rtspUri = rtspUri;
}

RTSP_Stream::~RTSP_Stream()
{
        this->Stop();
}

int RTSP_Stream::Start(QFrame* qframe)
{
    // this->Stop();
    this->_mediaPlayer.setSource(QUrl(this->_rtspUri.c_str()));
    this->_mediaWidget = new QVideoWidget(qframe);
    this->_mediaPlayer.setVideoOutput(this->_mediaWidget);
    this->_mediaPlayer.setPlaybackRate(0);
    this->_mediaWidget->setMinimumSize(qframe->size());
    this->_mediaWidget->setMaximumSize(qframe->size());
    this->_mediaWidget->show();
    this->_mediaPlayer.play();

    qDebug("Streaming RTSP - %s", this->_rtspUri);
    return IStream::Start(qframe);
}

void RTSP_Stream::Stop()
{
    this->_mediaPlayer.stop();
    if(this->_mediaWidget != NULL)
        delete this->_mediaWidget;

    IStream::Stop();
}

Playback_Stream::Playback_Stream(datetime from, datetime to, IChannel* channel, QWidget* parent) : IStream(channel, parent)
{
    this->_from = from;
    this->_to = to;
}

Playback_Stream::~Playback_Stream()
{

}

void Playback_Stream::TogglePause()
{
    this->_isPlaying = !this->_isPlaying;
}


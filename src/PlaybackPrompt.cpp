#include "../headers/PlaybackPrompt.h"

#include <QIntValidator>
#include <QDateTime>


PlaybackPrompt::PlaybackPrompt(Malenia* mainWindow, QWidget *parent): QWidget(parent)
{
	QDate startdate = QDateTime::currentDateTime().date();
	QDate stopdate = QDateTime::currentDateTime().date();
	QTime starttime(0, 0, 0);
	QTime stoptime(23, 59, 59);

    ui.setupUi(this);
    this->_mainWindow = mainWindow;

    this->_framesValidator.setBottom(1);
    ui.frameIndexInput->setValidator(&this->_framesValidator);

    // making it begin with beginning of today and end of today
	ui.fromDatetimeInput->setDate(startdate);
	ui.fromDatetimeInput->setTime(starttime);
	ui.toDatetimeInput->setDate(stopdate);
	ui.toDatetimeInput->setTime(stoptime);
}

PlaybackPrompt::~PlaybackPrompt()
{

}

void PlaybackPrompt::on_confirmButton_clicked()
{
    datetime from, to; 
    QDate fromDQ = ui.fromDatetimeInput->date(), toDQ = ui.toDatetimeInput->date();
    QTime fromTQ = ui.fromDatetimeInput->time(), toTQ = ui.toDatetimeInput->time();
    int frameIndex = ui.frameIndexInput->text().toInt() - 1;

    memset(&from, 0, sizeof(datetime));
    memset(&to, 0, sizeof(datetime));

    from.year = fromDQ.year();
    from.month = fromDQ.month();
    from.day = fromDQ.day();
    from.hours = fromTQ.hour();
    from.minutes = fromTQ.minute();
    from.seconds = 0;

    to.year = toDQ.year();
    to.month = toDQ.month();
    to.day = toDQ.day();
    to.hours = toTQ.hour();
    to.minutes = toTQ.minute();
    to.seconds = 0;

    this->_mainWindow->BeginPlayback(this->_channel, frameIndex, from, to);
    this->hide();
}

void PlaybackPrompt::Reset(IChannel* ch, int maxFrames)
{
    this->_channel = ch;
    ui.frameIndexInput->setText("1");
    this->_framesValidator.setTop(maxFrames);
}

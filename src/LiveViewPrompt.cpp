#include "../headers/LiveViewPrompt.h"

#include <QIntValidator>


LiveViewPrompt::LiveViewPrompt(Malenia* mainWindow, QWidget *parent): QWidget(parent)
{
    ui.setupUi(this);
    this->_mainWindow = mainWindow;

    this->_framesValidator.setBottom(1);
    ui.frameIndexInput->setValidator(&this->_framesValidator);
}

LiveViewPrompt::~LiveViewPrompt()
{

}

void LiveViewPrompt::on_confirmButton_clicked()
{
    this->_mainWindow->BeginLiveView(this->_channel, ui.frameIndexInput->text().toInt() - 1);
    this->hide();
}

void LiveViewPrompt::Reset(IChannel* ch, int maxFrames)
{
    this->_channel = ch;
    ui.frameIndexInput->setText("1");
    this->_framesValidator.setTop(maxFrames);
}

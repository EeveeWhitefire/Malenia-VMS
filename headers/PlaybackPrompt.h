#ifndef PLAYBACKPROMPT_H
#define PLAYBACKPROMPT_H

#include "ui_PlaybackPrompt.h"
#include <QFrame>

#include <vector>

class Malenia;
#include "../headers/IChannel.h"
#include "../headers/Malenia.h"


class PlaybackPrompt : public QWidget
{
    Q_OBJECT

public:
    PlaybackPrompt(Malenia*, QWidget* parent = 0);
    ~PlaybackPrompt();
    void Reset(IChannel*, int);

private slots:
    void on_confirmButton_clicked();
private:
    Ui::PlaybackPromptWindow ui;
    Malenia* _mainWindow = NULL;
    QIntValidator _framesValidator;

    std::vector<QFrame*>* _frames;
    IChannel* _channel = NULL;
};
#endif // PLAYBACKPROMPT_H

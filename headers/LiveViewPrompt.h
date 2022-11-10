#ifndef LIVEVIEWPROMPT_H
#define LIVEVIEWPROMPT_H

#include "ui_LiveViewPrompt.h"
#include <QFrame>

#include <vector>

class Malenia;
#include "../headers/IChannel.h"
#include "../headers/Malenia.h"


class LiveViewPrompt : public QWidget
{
    Q_OBJECT

public:
    LiveViewPrompt(Malenia*, QWidget* parent = 0);
    ~LiveViewPrompt();
    void Reset(IChannel*, int);

private slots:
    void on_confirmButton_clicked();
private:
    Ui::LiveViewPromptWindow ui;
    Malenia* _mainWindow = NULL;
    QIntValidator _framesValidator;

    std::vector<QFrame*>* _frames;
    IChannel* _channel = NULL;
};
#endif // PLAYBACKPROMPT_H

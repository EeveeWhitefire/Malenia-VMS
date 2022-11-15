#pragma once
#ifndef MALENIA_H
#define MALENIA_H

#include "ui_Malenia.h"
#include <QMainWindow>
#include <QPoint>
#include <QMouseEvent>
#include <QMediaPlayer>
#include <QAudioOutput>

#include <vector>
#include <map>

class LiveViewPrompt;
class PlaybackPrompt;
#include "../headers/IDevice.h"
#include "../headers/Database.h"
#include "../headers/LiveViewPrompt.h"
#include "../headers/PlaybackPrompt.h"

#define FRAMES_ROW_COUNT 4
#define FRAMES_COL_COUNT 4

class Malenia : public QMainWindow
{
    Q_OBJECT

public:
    Malenia(QWidget* parent = 0);
    ~Malenia();
    void BeginLiveView(IChannel*, int);
    void BeginPlayback(IChannel*, int, datetime, datetime);
    QFrame* getFrame(int);
    QFrame* getFreeFrame();
private slots:
    void on_connectButton_clicked();
    void on_autocompletePasswordButton_clicked();
    void connectionRightClick(const QPoint&);
    void connectionDoubleClick(QTreeWidgetItem*, int);
    void launchLiveView();
    void launchPlayback();
    void launchDeviceInfo();
    void launchPauseStream();
    void launchStopStream();
    void changeFramesLayout(int);
    void changeVendor(int);
protected slots:
    void mouseDoubleClickEvent(QMouseEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    // void resizeEvent(QResizeEvent* e) override;
private:
    Ui::MaleniaWindow ui;
    LiveViewPrompt* _liveViewPrompt = NULL;
    PlaybackPrompt* _playbackPrompt = NULL;
    QMediaPlayer _player;
    QAudioOutput _audioOutput;

    std::vector<QFrame*> _frames;
    std::map<QFrame*, IStream*> _streams;
    std::vector<IDevice*> _connections;
    std::map<QFrame*, QLabel*> _loadingMessages;

    Database _db;

    IDevice* _selectedDevice = NULL;
    IChannel* _selectedChannel = NULL;
    IStream* _selectedStream = NULL;

    QFrame* _fullscreenFrame = NULL;
    QPoint _fullscreenFrameOriginalPoint;
    QSize _defaultFrameSize;

    int _framesColCount = FRAMES_COL_COUNT;
    int _framesRowCount = FRAMES_ROW_COUNT;
    int _layoutWidth = 0;
    int _layoutHeight = 0;
};
#endif // MALENIA_H

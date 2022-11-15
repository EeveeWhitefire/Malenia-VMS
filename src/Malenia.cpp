#include "../headers/Malenia.h"
#include "../headers/Hikvision.h"
#include "../headers/Dahua.h"
#include "../headers/Provision.h"
#include "../headers/XMEye.h"
#include "../headers/utils.hpp"

#include <QIntValidator>
#include <QMenu>
#include <QFileInfo>
#include <QListView>


enum VENDOR_COMBOBOX_OPTIONS {HIKVISION_VENDOR_OPTION=0, DAHUA_VENDOR_OPTION, PROVISION_VENDOR_OPTION, XMEYE_VENDOR_OPTION};
enum CONNECTION_TYPE_COMBOBOX_OPTIONS {SDK_CONNECTION_OPTION=0, HTTP_CONNECTION_OPTION, P2P_CONNECTION_OPTION};
enum GRID_LAYOUT_COMBOBOX_OPTIONS {FOUR_BY_FOUR_OPTION=0, THREE_BY_THREE_OPTION=1, TWO_BY_TWO_OPTION};

Malenia::Malenia(QWidget *parent): QMainWindow(parent)
{
    int i = 0;

    ui.setupUi(this);
    this->setWindowIcon(QIcon("./assets/malenia-menu-icon.png")); // setting the icon for the process
    
    // setting up the prompts
    this->_liveViewPrompt = new LiveViewPrompt(this);
    this->_playbackPrompt = new PlaybackPrompt(this);
    
    for(i = 1; i < FRAMES_COL_COUNT*FRAMES_ROW_COUNT + 1; i++)
    {
        QFrame* f = ui.framesGrid->findChild<QFrame*>(QObject::tr("frame_%1").arg(i)); // getting the frame by its name. frame_n
        if(f != NULL)
        {
            // associating the pointers
            this->_frames.push_back(f);
            this->_streams[this->_frames[i-1]] = NULL;
            this->_loadingMessages[f] = f->findChild<QLabel*>(QObject::tr("loading_%1").arg(i)); // getting the LOADING message gui element
            this->_loadingMessages[f]->hide();
        }
    }

    this->_defaultFrameSize = this->_frames[0]->size();
    this->_layoutWidth = this->_defaultFrameSize.width() * FRAMES_COL_COUNT;
    this->_layoutHeight = this->_defaultFrameSize.height() * FRAMES_ROW_COUNT;

    // hooking the event of layout change
    connect(ui.layoutComboBox, SIGNAL(activated(int)), this, SLOT(changeFramesLayout(int)));

    ui.portInput->setValidator(new QIntValidator(1, 65535, this)); // port is a number between 1 and 65,535
    ui.treeWidget->setContextMenuPolicy(Qt::CustomContextMenu); // allowing me to have a custom right-click menu
    ui.treeWidget->header()->resizeSection(0, 190); // so the IP header of the devices tab will be bigger
    connect(ui.treeWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(connectionRightClick(QPoint))); //hooking the devices right click event
    connect(ui.treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(connectionDoubleClick(QTreeWidgetItem*, int))); // hooking the devices double click event

    // hooking the event of vendor change
    connect(ui.vendorComboBox, SIGNAL(activated(int)), this, SLOT(changeVendor(int)));
    changeVendor(0); // HIKVISION

    // HIKVISION SDK
    if(!NET_DVR_Init())
        MaleniaException::show(ERR_UNKNOWN, ("Hikvision SDK failed to load: " + std::to_string(NET_DVR_GetLastError())).c_str());
    else
        NET_DVR_SetConnectTime(10000, 1);

    // DAHUA SDK
    CLIENT_Init(NULL, NULL);
    
    // this plays when the process starts
    this->_audioOutput.setVolume(50);
    this->_player.setSource(QUrl::fromLocalFile("./assets/malenia-intro.mp3"));
    this->_player.setAudioOutput(&this->_audioOutput);
    this->_player.play();
}

Malenia::~Malenia()
{
    // cleanup
    for(IDevice* conn: this->_connections)
        delete conn;

    delete this->_liveViewPrompt;
    delete this->_playbackPrompt;

    // HIKVISION SDK
    NET_DVR_Cleanup();

    // DAHUA SDK
    CLIENT_Cleanup();
}

void Malenia::BeginLiveView(IChannel* channel, int frameIndex)
{
    // if the chosen frame is already playing something
    if(this->_streams[this->_frames[frameIndex]] != NULL)
    {
        this->_streams[this->_frames[frameIndex]]->getChannel()->StopStream(this->_streams[this->_frames[frameIndex]]); // so the channel itself will know it stopped streaming
        this->_streams[this->_frames[frameIndex]] = NULL;
    }

    // if the stream started successfully
    if(IStream* stream = channel->StartLiveView(this->_frames[frameIndex]); stream != NULL)
    {
        this->_loadingMessages[this->_frames[frameIndex]]->show();
        this->_streams[this->_frames[frameIndex]] = stream;
    }
}

void Malenia::BeginPlayback(IChannel* channel, int frameIndex, datetime from, datetime to)
{
    // if the chosen frame is already playing something
    if(this->_streams[this->_frames[frameIndex]] != NULL)
    {
        this->_streams[this->_frames[frameIndex]]->getChannel()->StopStream(this->_streams[this->_frames[frameIndex]]);
        this->_streams[this->_frames[frameIndex]] = NULL;
    }

    // if the stream started successfully
    if(IStream* stream = channel->StartPlayback(from, to, this->_frames[frameIndex]); stream != NULL)
    {
        this->_loadingMessages[this->_frames[frameIndex]]->show();
        this->_streams[this->_frames[frameIndex]] = stream;
    }
}

QFrame* Malenia::getFrame(int frameIndex)
{
    // get the frame at specified index
    return this->_frames[frameIndex];
}

QFrame* Malenia::getFreeFrame()
{
    int i = 0;

    // get the first frame that is not playing anything
    for(i = 0; i < this->_framesColCount*this->_framesRowCount; i++)
        if(this->_streams[this->_frames[i]] == NULL)
            return this->_frames[i];

    return NULL;
}

void Malenia::on_connectButton_clicked()
{
    int vendorOption = ui.vendorComboBox->currentIndex(), connTypeOption = ui.connTypeComboBox->currentIndex();
    char username[MAX_SHORT_STR_LEN] = {0}, password[MAX_SHORT_STR_LEN] = {0}, 
         sn[MAX_SHORT_STR_LEN] = {0};
    std::string ip = ui.ipInput->text().toLocal8Bit().data();
    int port = atoi(ui.portInput->text().toLocal8Bit().data());
    IDevice* device = NULL;

    strncpy_t(username, MAX_SHORT_STR_LEN, ui.usernameInput->text().toLocal8Bit().data(), MAX_SHORT_STR_LEN);
    strncpy_t(password, MAX_SHORT_STR_LEN, ui.passwordInput->text().toLocal8Bit().data(), MAX_SHORT_STR_LEN);
    
    trim(ip);
    
    if(!isValidIP(ip.c_str()))
        MaleniaException::show(ERR_INVALID_INPUT, "Invalid IP Address");
    else
    {
        switch(vendorOption)
        {
            case HIKVISION_VENDOR_OPTION:
                switch(connTypeOption)
                {
                    case SDK_CONNECTION_OPTION:
                        device = new Hikvision::SDK_Device(LOGIN_MODE_SDK, ip.c_str(), port);
                        break;
                    case HTTP_CONNECTION_OPTION:
                        device = new Hikvision::SDK_Device(LOGIN_MODE_HTTP, ip.c_str(), port);
                        break;
                }

                break;
            case DAHUA_VENDOR_OPTION:
                switch(connTypeOption)
                {
                    case SDK_CONNECTION_OPTION:
                        device = new Dahua::SDK_Device(LOGIN_MODE_SDK, ip.c_str(), port);
                        break;
                    case HTTP_CONNECTION_OPTION:
                        device = new Dahua::SDK_Device(LOGIN_MODE_HTTP, ip.c_str(), port);
                        break;
                }

                break;
            case PROVISION_VENDOR_OPTION:
                switch(connTypeOption)
                {
                    case SDK_CONNECTION_OPTION:
                        device = new Provision::SDK_Device(ip.c_str(), port);
                        break;
                    case HTTP_CONNECTION_OPTION:
                        device = new Provision::HTTP_Device(ip.c_str(), port);
                        break;
                }

                break;
            case XMEYE_VENDOR_OPTION:
                switch(connTypeOption)
                {
                    case HTTP_CONNECTION_OPTION:
                        device = new XMEye::HTTP_Device(ip.c_str(), port);
                        break;
                    case P2P_CONNECTION_OPTION:
                        device = XMEye::getDeviceFromNAT(sn);
                        break;
                }

                break;
        }
    }

    if(device != NULL)
    {
        if(device->login(username, password)) // if has login successfully
        {
            // add the device to the GUI
            ui.treeWidget->addTopLevelItem(device);
            this->_connections.push_back(device);
        }
        else
            delete device;
    }
}

void Malenia::on_autocompletePasswordButton_clicked()
{
    if(ui.ipInput->text().length() > 0)
    {
        JSONB res = this->_db.getPasswordIP(ui.ipInput->text().toLocal8Bit().data());
        if(std::map<std::string, std::string>::iterator it = res.getFirstPair(); !res.isEmpty()) // getting the first password
        {
            ui.portInput->setText(it->first.c_str());
            ui.passwordInput->setText(it->second.c_str());
        }
        else
            MaleniaException::show(ERR_AUTOCOMPLETE_NOT_FOUND);
    }
    else
        MaleniaException::show(ERR_AUTOCOMPLETE_PARAM_MISSING_IP);
}

void Malenia::connectionRightClick(const QPoint& pos)
{
    if(IChannel* ch = dynamic_cast<IChannel*>(ui.treeWidget->itemAt(pos)); ch != NULL)  // if you right click on a channel
    {
        this->_selectedChannel = ch;
        
        QMenu menu(tr("Channel Options"), this);
        menu.addAction("Toggle Live View", this, SLOT(launchLiveView()));
        menu.addAction("Playback", this, SLOT(launchPlayback()));
        menu.exec(ui.treeWidget->viewport()->mapToGlobal(pos)); 
    }
    else if(IDevice* dev = dynamic_cast<IDevice*>(ui.treeWidget->itemAt(pos)); dev != NULL) // if you right click on a device
    {
        this->_selectedDevice = dev;

        QMenu menu(tr("NVR Options"), this);
        menu.addAction("Show Information", this, SLOT(launchDeviceInfo()));
        menu.exec(ui.treeWidget->viewport()->mapToGlobal(pos));
    }
}

void Malenia::connectionDoubleClick(QTreeWidgetItem* item, int column)
{
    IChannel* ch = NULL;
    IStream* liveStream = NULL;
    std::vector<IChannel*> channels;

    if (ch = dynamic_cast<IChannel*>(item); ch != NULL) // if you double click on a channel
        channels.push_back(ch);
    else if (IDevice* conn = dynamic_cast<IDevice*>(item); conn != NULL) // if you double click on a device
    {
        ch = conn->getFreeChannel();
        while(ch != NULL)
        {
            channels.push_back(ch);
            ch = conn->getFreeChannel();
        }
    }

    for(IChannel* ch: channels)
    {
        if(liveStream = ch->getLiveStream(); liveStream != NULL) // if this channel is currently already playing a livestream
        {
            this->_loadingMessages[liveStream->getLastFrame()]->hide();
            this->_streams[liveStream->getLastFrame()] = NULL;
            ch->StopLiveStream();

        }
        else if(QFrame* f = this->getFreeFrame(); f != NULL) // if there is a free frame for a new live stream for this channel
        {
            if(liveStream = ch->StartLiveView(f); liveStream != NULL) // if starting live stream worked
            {
                this->_loadingMessages[f]->show();
                this->_streams[f] = liveStream;
            }
        }
        else
        {
            MaleniaException::show(ERR_NO_FREE_FRAMES);
            return;
        }
    }
}

void Malenia::mouseDoubleClickEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        if (QFrame* f = dynamic_cast<QFrame*>(this->childAt(e->pos())); f != NULL) // only if i double clicked on a frame
        {
            if(this->_fullscreenFrame == NULL) // if not on full screen mode currently
            {
                this->_fullscreenFrame = f;

                this->_fullscreenFrameOriginalPoint = this->_fullscreenFrame->pos();
                this->_fullscreenFrame->resize(this->_layoutWidth, this->_layoutHeight);
                this->_fullscreenFrame->move(10, 10);
                this->_fullscreenFrame->raise();
                this->_loadingMessages[this->_fullscreenFrame]->move(10, this->_layoutHeight - 30);
            }
            else // if on full screen mode currently
            {
                this->_fullscreenFrame->resize(this->_defaultFrameSize);
                this->_fullscreenFrame->move(this->_fullscreenFrameOriginalPoint);
                this->_fullscreenFrame->lower();
                this->_loadingMessages[this->_fullscreenFrame]->move(10, this->_fullscreenFrame->size().height() - 30);

                this->_fullscreenFrame = NULL;
            }

            if(this->_streams[f] != NULL && this->_streams[f]->isPlaying())
               this->_streams[f]->Replay(); // to alert the streams that the resolution changed
        }
    }

    QMainWindow::mouseDoubleClickEvent(e);
}

void Malenia::mousePressEvent(QMouseEvent* e)
{
    if(e->button() == Qt::RightButton)
    {
        if (QFrame* f = dynamic_cast<QFrame*>(this->childAt(e->pos())); f != NULL) // if you right clicked on a frame
        {
            if(this->_streams[f] != NULL) // only if this frame is playing a stream
            {
                this->_selectedStream = this->_streams[f];

                QMenu menu(tr("Stream Options"), this);
                if(dynamic_cast<Playback_Stream*>(this->_selectedStream) != NULL) // if is a playback stream
                    menu.addAction(this->_selectedStream->isPlaying() ? "Pause" : "Resume", this, SLOT(launchPauseStream()));
                menu.addAction("Stop", this, SLOT(launchStopStream()));
                menu.exec(e->globalPosition().toPoint());
            }
        }
    }

    QMainWindow::mousePressEvent(e);
}

void Malenia::launchLiveView()
{
    if(IStream* liveStream = this->_selectedChannel->getLiveStream(); liveStream != NULL) // if this channel is already playing live stream at some other frame
    {
        this->_loadingMessages[liveStream->getLastFrame()]->hide();
        this->_streams[liveStream->getLastFrame()] = NULL;
        this->_selectedChannel->StopLiveStream(); // stop it, no need for two live streams at the same time of the same channel
    }
    else
    {
        this->_liveViewPrompt->Reset(this->_selectedChannel, this->_framesColCount*this->_framesRowCount);
        this->_liveViewPrompt->show(); // show prompt
    }
}

void Malenia::launchPlayback()
{
    this->_playbackPrompt->Reset(this->_selectedChannel, this->_framesColCount*this->_framesRowCount);
    this->_playbackPrompt->show(); // show prompt
}

void Malenia::launchDeviceInfo()
{
    QMessageBox::information(this, 
                             tr("NVR %1:%2").arg(this->_selectedDevice->getIP()).arg(this->_selectedDevice->getPort()),
                             tr("Serial Number: %1\nModel Name: %2\nCH Amount: %3").arg(this->_selectedDevice->getSerialNumber()) \
                                                                                   .arg(this->_selectedDevice->getModelName()) \
                                                                                   .arg(this->_selectedDevice->getChannelsAmount()));
}

void Malenia::launchPauseStream()
{
    ((Playback_Stream*)this->_selectedStream)->TogglePause();
}

void Malenia::launchStopStream()
{
    this->_streams[this->_selectedStream->getLastFrame()] = NULL;
    this->_loadingMessages[this->_selectedStream->getLastFrame()]->hide();
    this->_selectedStream->getChannel()->StopStream(this->_selectedStream);
}

void Malenia::changeFramesLayout(int index)
{
    int i = 0, maxChannels = 0, rowC = 0, colC = 0, x = 0, y = 0;
    int newWidth = 0, newHeight = 0;
    
    switch(index)
    {
        case FOUR_BY_FOUR_OPTION:
            this->_framesColCount = 4;
            this->_framesRowCount = 4;
            break;
        case THREE_BY_THREE_OPTION:
            this->_framesColCount = 3;
            this->_framesRowCount = 3;
            break;
        case TWO_BY_TWO_OPTION:
            this->_framesColCount = 2;
            this->_framesRowCount = 2;
            break;
    }

    // calculating the new dimensions of each stream
    newWidth = (int)(this->_layoutWidth / this->_framesColCount);
    newHeight = (int)(this ->_layoutHeight / this->_framesRowCount);
    maxChannels = this->_framesColCount * this->_framesRowCount;

    this->_defaultFrameSize.setHeight(newHeight);
    this->_defaultFrameSize.setWidth(newWidth);

    for(i = 0; i < FRAMES_ROW_COUNT * FRAMES_COL_COUNT; i++)
    {
        if(i < maxChannels) // frames that are visible according to the layout (4x4 - first 16, 3x3 - first 9, etc)
        {
            // calculating each frame's position
            x = 10 + colC*newWidth;
            y = 10 + rowC*newHeight;

            // resizing stuff and moving it around to keep the gui nice
            this->_frames[i]->resize(newWidth, newHeight);
            this->_frames[i]->move(x, y);
            this->_loadingMessages[this->_frames[i]]->move(this->_loadingMessages[this->_frames[i]]->pos().x(), newHeight - 30);

            this->_frames[i]->show();
            if(this->_streams[this->_frames[i]] != NULL && this->_streams[this->_frames[i]]->isPlaying()) // if this frame is playing anything
                this->_streams[this->_frames[i]]->Replay(); // alerting the stream about the resolution change

            if(colC + 1 == this->_framesColCount)
            {
                rowC++;
                colC = 0;
            }
            else
                colC++;
        }
        else // frames that shouldn't be visible have their frames stopped and are hidden
        {    
            if(this->_streams[this->_frames[i]] != NULL)
            {
                this->_streams[this->_frames[i]]->getChannel()->StopStream(this->_streams[this->_frames[i]]);
                this->_streams[this->_frames[i]] = NULL;
                this->_loadingMessages[this->_frames[i]]->hide();
            }
            this->_frames[i]->hide();
        }
    }
}

void Malenia::changeVendor(int index)
{
    // updating the options for connection type of each vendor. 0 - shown, 1 - hidden
    switch(index)
    {
        case HIKVISION_VENDOR_OPTION:
            dynamic_cast<QListView*>(ui.connTypeComboBox->view())->setRowHidden(SDK_CONNECTION_OPTION, 0);
            dynamic_cast<QListView*>(ui.connTypeComboBox->view())->setRowHidden(HTTP_CONNECTION_OPTION, 0);
            dynamic_cast<QListView*>(ui.connTypeComboBox->view())->setRowHidden(P2P_CONNECTION_OPTION, 1);

            ui.connTypeComboBox->setCurrentIndex(SDK_CONNECTION_OPTION);
            break;
        case DAHUA_VENDOR_OPTION:
            dynamic_cast<QListView*>(ui.connTypeComboBox->view())->setRowHidden(SDK_CONNECTION_OPTION, 0);
            dynamic_cast<QListView*>(ui.connTypeComboBox->view())->setRowHidden(HTTP_CONNECTION_OPTION, 0);
            dynamic_cast<QListView*>(ui.connTypeComboBox->view())->setRowHidden(P2P_CONNECTION_OPTION, 1);

            ui.connTypeComboBox->setCurrentIndex(SDK_CONNECTION_OPTION);
            break;
        case PROVISION_VENDOR_OPTION:
            dynamic_cast<QListView*>(ui.connTypeComboBox->view())->setRowHidden(SDK_CONNECTION_OPTION, 0);
            dynamic_cast<QListView*>(ui.connTypeComboBox->view())->setRowHidden(HTTP_CONNECTION_OPTION, 0);
            dynamic_cast<QListView*>(ui.connTypeComboBox->view())->setRowHidden(P2P_CONNECTION_OPTION, 1);

            ui.connTypeComboBox->setCurrentIndex(SDK_CONNECTION_OPTION);
            break;
        case XMEYE_VENDOR_OPTION:
            dynamic_cast<QListView*>(ui.connTypeComboBox->view())->setRowHidden(SDK_CONNECTION_OPTION, 1);
            dynamic_cast<QListView*>(ui.connTypeComboBox->view())->setRowHidden(HTTP_CONNECTION_OPTION, 0);
            dynamic_cast<QListView*>(ui.connTypeComboBox->view())->setRowHidden(P2P_CONNECTION_OPTION, 0);

            ui.connTypeComboBox->setCurrentIndex(HTTP_CONNECTION_OPTION);
            break;
    }
}

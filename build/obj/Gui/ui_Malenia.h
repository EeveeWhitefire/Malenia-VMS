/********************************************************************************
** Form generated from reading UI file 'Malenia.ui'
**
** Created by: Qt User Interface Compiler version 6.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MALENIA_H
#define UI_MALENIA_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MaleniaWindow
{
public:
    QWidget *centralwidget;
    QWidget *framesGrid;
    QFrame *frame_1;
    QLabel *label_8;
    QLabel *loading_1;
    QFrame *frame_4;
    QLabel *label_12;
    QLabel *loading_4;
    QFrame *frame_2;
    QLabel *label_10;
    QLabel *loading_2;
    QFrame *frame_5;
    QLabel *label_13;
    QLabel *loading_5;
    QFrame *frame_3;
    QLabel *label_11;
    QLabel *loading_3;
    QFrame *frame_6;
    QLabel *label_14;
    QLabel *loading_6;
    QFrame *frame_7;
    QLabel *label_15;
    QLabel *loading_7;
    QFrame *frame_8;
    QLabel *label_16;
    QLabel *loading_8;
    QFrame *frame_9;
    QLabel *label_17;
    QLabel *loading_9;
    QFrame *frame_10;
    QLabel *label_18;
    QLabel *loading_10;
    QFrame *frame_11;
    QLabel *label_19;
    QLabel *loading_11;
    QFrame *frame_12;
    QLabel *label_20;
    QLabel *loading_12;
    QFrame *frame_13;
    QLabel *label_24;
    QLabel *loading_13;
    QFrame *frame_14;
    QLabel *label_23;
    QLabel *loading_14;
    QFrame *frame_15;
    QLabel *label_22;
    QLabel *loading_15;
    QFrame *frame_16;
    QLabel *label_21;
    QLabel *loading_16;
    QLabel *logo_pic;
    QLabel *logo_name;
    QWidget *widget;
    QPushButton *connectButton;
    QLineEdit *ipInput;
    QComboBox *vendorComboBox;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label;
    QComboBox *connTypeComboBox;
    QLabel *label_3;
    QLineEdit *portInput;
    QLineEdit *usernameInput;
    QLabel *label_2;
    QLineEdit *passwordInput;
    QLabel *label_7;
    QComboBox *layoutComboBox;
    QPushButton *autocompletePasswordButton;
    QWidget *widget_10;
    QTreeWidget *treeWidget;
    QLabel *label_9;

    void setupUi(QMainWindow *MaleniaWindow)
    {
        if (MaleniaWindow->objectName().isEmpty())
            MaleniaWindow->setObjectName("MaleniaWindow");
        MaleniaWindow->resize(2261, 1241);
        MaleniaWindow->setMinimumSize(QSize(2261, 1241));
        MaleniaWindow->setMaximumSize(QSize(2261, 1241));
        MaleniaWindow->setStyleSheet(QString::fromUtf8("background-color: rgb(45, 45, 45);"));
        centralwidget = new QWidget(MaleniaWindow);
        centralwidget->setObjectName("centralwidget");
        framesGrid = new QWidget(centralwidget);
        framesGrid->setObjectName("framesGrid");
        framesGrid->setGeometry(QRect(310, 130, 1941, 1101));
        framesGrid->setStyleSheet(QString::fromUtf8("background-color: rgb(59, 22, 22);"));
        frame_1 = new QFrame(framesGrid);
        frame_1->setObjectName("frame_1");
        frame_1->setGeometry(QRect(10, 10, 480, 270));
        frame_1->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        frame_1->setFrameShape(QFrame::Box);
        frame_1->setFrameShadow(QFrame::Raised);
        frame_1->setLineWidth(1);
        frame_1->setMidLineWidth(1);
        label_8 = new QLabel(frame_1);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(10, 10, 31, 21));
        QFont font;
        font.setPointSize(16);
        label_8->setFont(font);
        label_8->setStyleSheet(QString::fromUtf8("color: rgb(234, 234, 234);"));
        loading_1 = new QLabel(frame_1);
        loading_1->setObjectName("loading_1");
        loading_1->setGeometry(QRect(10, 240, 111, 22));
        loading_1->setFont(font);
        loading_1->setStyleSheet(QString::fromUtf8("color: rgb(234, 234, 234);"));
        frame_4 = new QFrame(framesGrid);
        frame_4->setObjectName("frame_4");
        frame_4->setGeometry(QRect(1450, 10, 480, 270));
        frame_4->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        frame_4->setFrameShape(QFrame::Box);
        frame_4->setFrameShadow(QFrame::Raised);
        frame_4->setMidLineWidth(1);
        label_12 = new QLabel(frame_4);
        label_12->setObjectName("label_12");
        label_12->setGeometry(QRect(10, 10, 31, 21));
        label_12->setFont(font);
        label_12->setStyleSheet(QString::fromUtf8("color: rgb(234, 234, 234);"));
        loading_4 = new QLabel(frame_4);
        loading_4->setObjectName("loading_4");
        loading_4->setGeometry(QRect(10, 240, 111, 22));
        loading_4->setFont(font);
        loading_4->setStyleSheet(QString::fromUtf8("color: rgb(234, 234, 234);"));
        frame_2 = new QFrame(framesGrid);
        frame_2->setObjectName("frame_2");
        frame_2->setGeometry(QRect(490, 10, 480, 270));
        frame_2->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        frame_2->setFrameShape(QFrame::Box);
        frame_2->setFrameShadow(QFrame::Raised);
        frame_2->setMidLineWidth(1);
        label_10 = new QLabel(frame_2);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(10, 10, 31, 21));
        label_10->setFont(font);
        label_10->setStyleSheet(QString::fromUtf8("color: rgb(234, 234, 234);"));
        loading_2 = new QLabel(frame_2);
        loading_2->setObjectName("loading_2");
        loading_2->setGeometry(QRect(10, 240, 111, 22));
        loading_2->setFont(font);
        loading_2->setStyleSheet(QString::fromUtf8("color: rgb(234, 234, 234);"));
        frame_5 = new QFrame(framesGrid);
        frame_5->setObjectName("frame_5");
        frame_5->setGeometry(QRect(10, 280, 480, 270));
        frame_5->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        frame_5->setFrameShape(QFrame::Box);
        frame_5->setFrameShadow(QFrame::Raised);
        frame_5->setMidLineWidth(1);
        label_13 = new QLabel(frame_5);
        label_13->setObjectName("label_13");
        label_13->setGeometry(QRect(10, 10, 31, 21));
        label_13->setFont(font);
        label_13->setStyleSheet(QString::fromUtf8("color: rgb(234, 234, 234);"));
        loading_5 = new QLabel(frame_5);
        loading_5->setObjectName("loading_5");
        loading_5->setGeometry(QRect(10, 240, 111, 22));
        loading_5->setFont(font);
        loading_5->setStyleSheet(QString::fromUtf8("color: rgb(234, 234, 234);"));
        frame_3 = new QFrame(framesGrid);
        frame_3->setObjectName("frame_3");
        frame_3->setGeometry(QRect(970, 10, 480, 270));
        frame_3->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        frame_3->setFrameShape(QFrame::Box);
        frame_3->setFrameShadow(QFrame::Raised);
        frame_3->setMidLineWidth(1);
        label_11 = new QLabel(frame_3);
        label_11->setObjectName("label_11");
        label_11->setGeometry(QRect(10, 10, 31, 21));
        label_11->setFont(font);
        label_11->setStyleSheet(QString::fromUtf8("color: rgb(234, 234, 234);"));
        loading_3 = new QLabel(frame_3);
        loading_3->setObjectName("loading_3");
        loading_3->setGeometry(QRect(10, 240, 111, 22));
        loading_3->setFont(font);
        loading_3->setStyleSheet(QString::fromUtf8("color: rgb(234, 234, 234);"));
        frame_6 = new QFrame(framesGrid);
        frame_6->setObjectName("frame_6");
        frame_6->setGeometry(QRect(490, 280, 480, 270));
        frame_6->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        frame_6->setFrameShape(QFrame::Box);
        frame_6->setFrameShadow(QFrame::Raised);
        frame_6->setMidLineWidth(1);
        label_14 = new QLabel(frame_6);
        label_14->setObjectName("label_14");
        label_14->setGeometry(QRect(10, 10, 31, 21));
        label_14->setFont(font);
        label_14->setStyleSheet(QString::fromUtf8("color: rgb(234, 234, 234);"));
        loading_6 = new QLabel(frame_6);
        loading_6->setObjectName("loading_6");
        loading_6->setGeometry(QRect(10, 240, 111, 22));
        loading_6->setFont(font);
        loading_6->setStyleSheet(QString::fromUtf8("color: rgb(234, 234, 234);"));
        frame_7 = new QFrame(framesGrid);
        frame_7->setObjectName("frame_7");
        frame_7->setGeometry(QRect(970, 280, 480, 270));
        frame_7->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        frame_7->setFrameShape(QFrame::Box);
        frame_7->setFrameShadow(QFrame::Raised);
        frame_7->setMidLineWidth(1);
        label_15 = new QLabel(frame_7);
        label_15->setObjectName("label_15");
        label_15->setGeometry(QRect(10, 10, 31, 21));
        label_15->setFont(font);
        label_15->setStyleSheet(QString::fromUtf8("color: rgb(234, 234, 234);"));
        loading_7 = new QLabel(frame_7);
        loading_7->setObjectName("loading_7");
        loading_7->setGeometry(QRect(10, 240, 111, 22));
        loading_7->setFont(font);
        loading_7->setStyleSheet(QString::fromUtf8("color: rgb(234, 234, 234);"));
        frame_8 = new QFrame(framesGrid);
        frame_8->setObjectName("frame_8");
        frame_8->setGeometry(QRect(1450, 280, 480, 270));
        frame_8->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        frame_8->setFrameShape(QFrame::Box);
        frame_8->setFrameShadow(QFrame::Raised);
        frame_8->setMidLineWidth(1);
        label_16 = new QLabel(frame_8);
        label_16->setObjectName("label_16");
        label_16->setGeometry(QRect(10, 10, 31, 21));
        label_16->setFont(font);
        label_16->setStyleSheet(QString::fromUtf8("color: rgb(234, 234, 234);"));
        loading_8 = new QLabel(frame_8);
        loading_8->setObjectName("loading_8");
        loading_8->setGeometry(QRect(10, 240, 111, 22));
        loading_8->setFont(font);
        loading_8->setStyleSheet(QString::fromUtf8("color: rgb(234, 234, 234);"));
        frame_9 = new QFrame(framesGrid);
        frame_9->setObjectName("frame_9");
        frame_9->setGeometry(QRect(10, 550, 480, 270));
        frame_9->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        frame_9->setFrameShape(QFrame::Box);
        frame_9->setFrameShadow(QFrame::Raised);
        frame_9->setMidLineWidth(1);
        label_17 = new QLabel(frame_9);
        label_17->setObjectName("label_17");
        label_17->setGeometry(QRect(10, 10, 31, 21));
        label_17->setFont(font);
        label_17->setStyleSheet(QString::fromUtf8("color: rgb(234, 234, 234);"));
        loading_9 = new QLabel(frame_9);
        loading_9->setObjectName("loading_9");
        loading_9->setGeometry(QRect(10, 240, 111, 22));
        loading_9->setFont(font);
        loading_9->setStyleSheet(QString::fromUtf8("color: rgb(234, 234, 234);"));
        frame_10 = new QFrame(framesGrid);
        frame_10->setObjectName("frame_10");
        frame_10->setGeometry(QRect(490, 550, 480, 270));
        frame_10->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        frame_10->setFrameShape(QFrame::Box);
        frame_10->setFrameShadow(QFrame::Raised);
        frame_10->setMidLineWidth(1);
        label_18 = new QLabel(frame_10);
        label_18->setObjectName("label_18");
        label_18->setGeometry(QRect(10, 10, 31, 21));
        label_18->setFont(font);
        label_18->setStyleSheet(QString::fromUtf8("color: rgb(234, 234, 234);"));
        loading_10 = new QLabel(frame_10);
        loading_10->setObjectName("loading_10");
        loading_10->setGeometry(QRect(10, 240, 111, 22));
        loading_10->setFont(font);
        loading_10->setStyleSheet(QString::fromUtf8("color: rgb(234, 234, 234);"));
        frame_11 = new QFrame(framesGrid);
        frame_11->setObjectName("frame_11");
        frame_11->setGeometry(QRect(970, 550, 480, 270));
        frame_11->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        frame_11->setFrameShape(QFrame::Box);
        frame_11->setFrameShadow(QFrame::Raised);
        frame_11->setMidLineWidth(1);
        label_19 = new QLabel(frame_11);
        label_19->setObjectName("label_19");
        label_19->setGeometry(QRect(10, 10, 31, 21));
        label_19->setFont(font);
        label_19->setStyleSheet(QString::fromUtf8("color: rgb(234, 234, 234);"));
        loading_11 = new QLabel(frame_11);
        loading_11->setObjectName("loading_11");
        loading_11->setGeometry(QRect(10, 240, 111, 22));
        loading_11->setFont(font);
        loading_11->setStyleSheet(QString::fromUtf8("color: rgb(234, 234, 234);"));
        frame_12 = new QFrame(framesGrid);
        frame_12->setObjectName("frame_12");
        frame_12->setGeometry(QRect(1450, 550, 480, 270));
        frame_12->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        frame_12->setFrameShape(QFrame::Box);
        frame_12->setFrameShadow(QFrame::Raised);
        frame_12->setMidLineWidth(1);
        label_20 = new QLabel(frame_12);
        label_20->setObjectName("label_20");
        label_20->setGeometry(QRect(10, 10, 31, 21));
        label_20->setFont(font);
        label_20->setStyleSheet(QString::fromUtf8("color: rgb(234, 234, 234);"));
        loading_12 = new QLabel(frame_12);
        loading_12->setObjectName("loading_12");
        loading_12->setGeometry(QRect(10, 240, 111, 22));
        loading_12->setFont(font);
        loading_12->setStyleSheet(QString::fromUtf8("color: rgb(234, 234, 234);"));
        frame_13 = new QFrame(framesGrid);
        frame_13->setObjectName("frame_13");
        frame_13->setGeometry(QRect(10, 820, 480, 270));
        frame_13->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        frame_13->setFrameShape(QFrame::Box);
        frame_13->setFrameShadow(QFrame::Raised);
        frame_13->setMidLineWidth(1);
        label_24 = new QLabel(frame_13);
        label_24->setObjectName("label_24");
        label_24->setGeometry(QRect(10, 10, 31, 21));
        label_24->setFont(font);
        label_24->setStyleSheet(QString::fromUtf8("color: rgb(234, 234, 234);"));
        loading_13 = new QLabel(frame_13);
        loading_13->setObjectName("loading_13");
        loading_13->setGeometry(QRect(10, 240, 111, 22));
        loading_13->setFont(font);
        loading_13->setStyleSheet(QString::fromUtf8("color: rgb(234, 234, 234);"));
        frame_14 = new QFrame(framesGrid);
        frame_14->setObjectName("frame_14");
        frame_14->setGeometry(QRect(490, 820, 480, 270));
        frame_14->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        frame_14->setFrameShape(QFrame::Box);
        frame_14->setFrameShadow(QFrame::Raised);
        frame_14->setMidLineWidth(1);
        label_23 = new QLabel(frame_14);
        label_23->setObjectName("label_23");
        label_23->setGeometry(QRect(10, 10, 31, 21));
        label_23->setFont(font);
        label_23->setStyleSheet(QString::fromUtf8("color: rgb(234, 234, 234);"));
        loading_14 = new QLabel(frame_14);
        loading_14->setObjectName("loading_14");
        loading_14->setGeometry(QRect(10, 240, 111, 22));
        loading_14->setFont(font);
        loading_14->setStyleSheet(QString::fromUtf8("color: rgb(234, 234, 234);"));
        frame_15 = new QFrame(framesGrid);
        frame_15->setObjectName("frame_15");
        frame_15->setGeometry(QRect(970, 820, 480, 270));
        frame_15->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        frame_15->setFrameShape(QFrame::Box);
        frame_15->setFrameShadow(QFrame::Raised);
        frame_15->setMidLineWidth(1);
        label_22 = new QLabel(frame_15);
        label_22->setObjectName("label_22");
        label_22->setGeometry(QRect(10, 10, 31, 21));
        label_22->setFont(font);
        label_22->setStyleSheet(QString::fromUtf8("color: rgb(234, 234, 234);"));
        loading_15 = new QLabel(frame_15);
        loading_15->setObjectName("loading_15");
        loading_15->setGeometry(QRect(10, 240, 111, 22));
        loading_15->setFont(font);
        loading_15->setStyleSheet(QString::fromUtf8("color: rgb(234, 234, 234);"));
        frame_16 = new QFrame(framesGrid);
        frame_16->setObjectName("frame_16");
        frame_16->setGeometry(QRect(1450, 820, 480, 270));
        frame_16->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        frame_16->setFrameShape(QFrame::Box);
        frame_16->setFrameShadow(QFrame::Raised);
        frame_16->setMidLineWidth(1);
        label_21 = new QLabel(frame_16);
        label_21->setObjectName("label_21");
        label_21->setGeometry(QRect(10, 10, 31, 21));
        label_21->setFont(font);
        label_21->setStyleSheet(QString::fromUtf8("color: rgb(234, 234, 234);"));
        loading_16 = new QLabel(frame_16);
        loading_16->setObjectName("loading_16");
        loading_16->setGeometry(QRect(10, 240, 111, 22));
        loading_16->setFont(font);
        loading_16->setStyleSheet(QString::fromUtf8("color: rgb(234, 234, 234);"));
        logo_pic = new QLabel(centralwidget);
        logo_pic->setObjectName("logo_pic");
        logo_pic->setGeometry(QRect(10, 10, 121, 101));
        logo_pic->setPixmap(QPixmap(QString::fromUtf8("../build/assets/malenia-menu-icon.png")));
        logo_pic->setScaledContents(true);
        logo_name = new QLabel(centralwidget);
        logo_name->setObjectName("logo_name");
        logo_name->setGeometry(QRect(130, 20, 201, 51));
        logo_name->setStyleSheet(QString::fromUtf8("color: rgb(171, 141, 21);"));
        widget = new QWidget(centralwidget);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(410, 10, 1841, 91));
        widget->setStyleSheet(QString::fromUtf8("background-color: rgb(59, 22, 22);"));
        connectButton = new QPushButton(widget);
        connectButton->setObjectName("connectButton");
        connectButton->setGeometry(QRect(1670, 10, 161, 71));
        QFont font1;
        font1.setPointSize(24);
        connectButton->setFont(font1);
        connectButton->setStyleSheet(QString::fromUtf8("background-color: rgb(171, 141, 21);\n"
"color: rgb(45, 45, 45);"));
        ipInput = new QLineEdit(widget);
        ipInput->setObjectName("ipInput");
        ipInput->setGeometry(QRect(350, 10, 211, 31));
        QFont font2;
        font2.setPointSize(12);
        ipInput->setFont(font2);
        ipInput->setStyleSheet(QString::fromUtf8("background-color: rgb(74, 74, 74);\n"
"color: rgb(212, 212, 212);"));
        vendorComboBox = new QComboBox(widget);
        vendorComboBox->addItem(QString());
        vendorComboBox->addItem(QString());
        vendorComboBox->addItem(QString());
        vendorComboBox->addItem(QString());
        vendorComboBox->setObjectName("vendorComboBox");
        vendorComboBox->setGeometry(QRect(30, 40, 131, 35));
        vendorComboBox->setFont(font2);
        vendorComboBox->setStyleSheet(QString::fromUtf8("background-color: rgb(74, 74, 74);\n"
"color: rgb(234, 234, 234);\n"
""));
        label_4 = new QLabel(widget);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(290, 50, 91, 21));
        label_4->setFont(font2);
        label_4->setStyleSheet(QString::fromUtf8("color: rgb(234, 234, 234);"));
        label_5 = new QLabel(widget);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(180, 10, 91, 21));
        label_5->setFont(font2);
        label_5->setStyleSheet(QString::fromUtf8("color: rgb(234, 234, 234);"));
        label_6 = new QLabel(widget);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(30, 10, 91, 21));
        label_6->setFont(font2);
        label_6->setStyleSheet(QString::fromUtf8("color: rgb(234, 234, 234);"));
        label = new QLabel(widget);
        label->setObjectName("label");
        label->setGeometry(QRect(630, 10, 91, 21));
        label->setFont(font2);
        label->setStyleSheet(QString::fromUtf8("color: rgb(234, 234, 234);"));
        connTypeComboBox = new QComboBox(widget);
        connTypeComboBox->addItem(QString());
        connTypeComboBox->addItem(QString());
        connTypeComboBox->addItem(QString());
        connTypeComboBox->setObjectName("connTypeComboBox");
        connTypeComboBox->setGeometry(QRect(180, 40, 81, 35));
        connTypeComboBox->setFont(font2);
        connTypeComboBox->setStyleSheet(QString::fromUtf8("background-color: rgb(74, 74, 74);\n"
"color: rgb(234, 234, 234);"));
        label_3 = new QLabel(widget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(290, 10, 51, 21));
        label_3->setFont(font2);
        label_3->setStyleSheet(QString::fromUtf8("color: rgb(234, 234, 234);"));
        portInput = new QLineEdit(widget);
        portInput->setObjectName("portInput");
        portInput->setGeometry(QRect(350, 50, 81, 31));
        portInput->setFont(font2);
        portInput->setStyleSheet(QString::fromUtf8("background-color: rgb(74, 74, 74);\n"
"color: rgb(212, 212, 212);"));
        usernameInput = new QLineEdit(widget);
        usernameInput->setObjectName("usernameInput");
        usernameInput->setGeometry(QRect(730, 10, 131, 31));
        usernameInput->setFont(font2);
        usernameInput->setStyleSheet(QString::fromUtf8("background-color: rgb(74, 74, 74);\n"
"color: rgb(234, 234, 234);"));
        label_2 = new QLabel(widget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(630, 50, 91, 21));
        label_2->setFont(font2);
        label_2->setStyleSheet(QString::fromUtf8("color: rgb(234, 234, 234);"));
        passwordInput = new QLineEdit(widget);
        passwordInput->setObjectName("passwordInput");
        passwordInput->setGeometry(QRect(730, 50, 131, 31));
        passwordInput->setFont(font2);
        passwordInput->setStyleSheet(QString::fromUtf8("background-color: rgb(74, 74, 74);\n"
"color: rgb(234, 234, 234);"));
        label_7 = new QLabel(widget);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(1550, 10, 91, 21));
        label_7->setFont(font2);
        label_7->setStyleSheet(QString::fromUtf8("color: rgb(234, 234, 234);"));
        layoutComboBox = new QComboBox(widget);
        layoutComboBox->addItem(QString());
        layoutComboBox->addItem(QString());
        layoutComboBox->addItem(QString());
        layoutComboBox->setObjectName("layoutComboBox");
        layoutComboBox->setGeometry(QRect(1550, 40, 71, 35));
        layoutComboBox->setFont(font2);
        layoutComboBox->setStyleSheet(QString::fromUtf8("background-color: rgb(74, 74, 74);\n"
"color: rgb(234, 234, 234);"));
        autocompletePasswordButton = new QPushButton(widget);
        autocompletePasswordButton->setObjectName("autocompletePasswordButton");
        autocompletePasswordButton->setGeometry(QRect(880, 10, 191, 71));
        autocompletePasswordButton->setFont(font);
        autocompletePasswordButton->setStyleSheet(QString::fromUtf8("background-color: rgb(171, 141, 21);\n"
"color: rgb(45, 45, 45);"));
        widget_10 = new QWidget(centralwidget);
        widget_10->setObjectName("widget_10");
        widget_10->setGeometry(QRect(10, 130, 291, 1101));
        widget_10->setStyleSheet(QString::fromUtf8("background-color: rgb(59, 22, 22);"));
        treeWidget = new QTreeWidget(widget_10);
        QBrush brush(QColor(234, 234, 234, 255));
        brush.setStyle(Qt::SolidPattern);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setFont(1, font2);
        __qtreewidgetitem->setBackground(1, QColor(45, 45, 45));
        __qtreewidgetitem->setForeground(1, brush);
        __qtreewidgetitem->setFont(0, font2);
        __qtreewidgetitem->setBackground(0, QColor(45, 45, 45));
        __qtreewidgetitem->setForeground(0, brush);
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName("treeWidget");
        treeWidget->setGeometry(QRect(10, 40, 271, 1051));
        treeWidget->setFont(font2);
        treeWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(74, 74, 74);"));
        label_9 = new QLabel(widget_10);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(10, 10, 121, 21));
        label_9->setFont(font2);
        label_9->setStyleSheet(QString::fromUtf8("color: rgb(171, 141, 21);"));
        MaleniaWindow->setCentralWidget(centralwidget);

        retranslateUi(MaleniaWindow);

        QMetaObject::connectSlotsByName(MaleniaWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MaleniaWindow)
    {
        MaleniaWindow->setWindowTitle(QCoreApplication::translate("MaleniaWindow", "Malenia VMS", nullptr));
        label_8->setText(QCoreApplication::translate("MaleniaWindow", "1", nullptr));
        loading_1->setText(QCoreApplication::translate("MaleniaWindow", "Loading", nullptr));
        label_12->setText(QCoreApplication::translate("MaleniaWindow", "4", nullptr));
        loading_4->setText(QCoreApplication::translate("MaleniaWindow", "Loading", nullptr));
        label_10->setText(QCoreApplication::translate("MaleniaWindow", "2", nullptr));
        loading_2->setText(QCoreApplication::translate("MaleniaWindow", "Loading", nullptr));
        label_13->setText(QCoreApplication::translate("MaleniaWindow", "5", nullptr));
        loading_5->setText(QCoreApplication::translate("MaleniaWindow", "Loading", nullptr));
        label_11->setText(QCoreApplication::translate("MaleniaWindow", "3", nullptr));
        loading_3->setText(QCoreApplication::translate("MaleniaWindow", "Loading", nullptr));
        label_14->setText(QCoreApplication::translate("MaleniaWindow", "6", nullptr));
        loading_6->setText(QCoreApplication::translate("MaleniaWindow", "Loading", nullptr));
        label_15->setText(QCoreApplication::translate("MaleniaWindow", "7", nullptr));
        loading_7->setText(QCoreApplication::translate("MaleniaWindow", "Loading", nullptr));
        label_16->setText(QCoreApplication::translate("MaleniaWindow", "8", nullptr));
        loading_8->setText(QCoreApplication::translate("MaleniaWindow", "Loading", nullptr));
        label_17->setText(QCoreApplication::translate("MaleniaWindow", "9", nullptr));
        loading_9->setText(QCoreApplication::translate("MaleniaWindow", "Loading", nullptr));
        label_18->setText(QCoreApplication::translate("MaleniaWindow", "10", nullptr));
        loading_10->setText(QCoreApplication::translate("MaleniaWindow", "Loading", nullptr));
        label_19->setText(QCoreApplication::translate("MaleniaWindow", "11", nullptr));
        loading_11->setText(QCoreApplication::translate("MaleniaWindow", "Loading", nullptr));
        label_20->setText(QCoreApplication::translate("MaleniaWindow", "12", nullptr));
        loading_12->setText(QCoreApplication::translate("MaleniaWindow", "Loading", nullptr));
        label_24->setText(QCoreApplication::translate("MaleniaWindow", "13", nullptr));
        loading_13->setText(QCoreApplication::translate("MaleniaWindow", "Loading", nullptr));
        label_23->setText(QCoreApplication::translate("MaleniaWindow", "14", nullptr));
        loading_14->setText(QCoreApplication::translate("MaleniaWindow", "Loading", nullptr));
        label_22->setText(QCoreApplication::translate("MaleniaWindow", "15", nullptr));
        loading_15->setText(QCoreApplication::translate("MaleniaWindow", "Loading", nullptr));
        label_21->setText(QCoreApplication::translate("MaleniaWindow", "16", nullptr));
        loading_16->setText(QCoreApplication::translate("MaleniaWindow", "Loading", nullptr));
        logo_pic->setText(QString());
        logo_name->setText(QCoreApplication::translate("MaleniaWindow", "<html><head/><body><p><span style=\" font-size:22pt;\">Malenia VMS</span></p></body></html>", nullptr));
        connectButton->setText(QCoreApplication::translate("MaleniaWindow", "CONNECT", nullptr));
        vendorComboBox->setItemText(0, QCoreApplication::translate("MaleniaWindow", " HIKVISION", nullptr));
        vendorComboBox->setItemText(1, QCoreApplication::translate("MaleniaWindow", " DAHUA", nullptr));
        vendorComboBox->setItemText(2, QCoreApplication::translate("MaleniaWindow", " PROVISION", nullptr));
        vendorComboBox->setItemText(3, QCoreApplication::translate("MaleniaWindow", " XMEYE", nullptr));

        label_4->setText(QCoreApplication::translate("MaleniaWindow", "Port:", nullptr));
        label_5->setText(QCoreApplication::translate("MaleniaWindow", "Type:", nullptr));
        label_6->setText(QCoreApplication::translate("MaleniaWindow", "Vendor:", nullptr));
        label->setText(QCoreApplication::translate("MaleniaWindow", "Username:", nullptr));
        connTypeComboBox->setItemText(0, QCoreApplication::translate("MaleniaWindow", " SDK", nullptr));
        connTypeComboBox->setItemText(1, QCoreApplication::translate("MaleniaWindow", " HTTP", nullptr));
        connTypeComboBox->setItemText(2, QCoreApplication::translate("MaleniaWindow", " P2P", nullptr));

        label_3->setText(QCoreApplication::translate("MaleniaWindow", "IP:", nullptr));
        usernameInput->setText(QCoreApplication::translate("MaleniaWindow", "admin", nullptr));
        label_2->setText(QCoreApplication::translate("MaleniaWindow", "Password:", nullptr));
        label_7->setText(QCoreApplication::translate("MaleniaWindow", "Grid:", nullptr));
        layoutComboBox->setItemText(0, QCoreApplication::translate("MaleniaWindow", " 4x4", nullptr));
        layoutComboBox->setItemText(1, QCoreApplication::translate("MaleniaWindow", " 3x3", nullptr));
        layoutComboBox->setItemText(2, QCoreApplication::translate("MaleniaWindow", " 2x2", nullptr));

        autocompletePasswordButton->setText(QCoreApplication::translate("MaleniaWindow", "AUTOCOMPLETE\n"
"PASSWORD", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(1, QCoreApplication::translate("MaleniaWindow", "PORT", nullptr));
        ___qtreewidgetitem->setText(0, QCoreApplication::translate("MaleniaWindow", "IP", nullptr));
        label_9->setText(QCoreApplication::translate("MaleniaWindow", "Connections:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MaleniaWindow: public Ui_MaleniaWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MALENIA_H

/********************************************************************************
** Form generated from reading UI file 'PlaybackPrompt.ui'
**
** Created by: Qt User Interface Compiler version 6.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLAYBACKPROMPT_H
#define UI_PLAYBACKPROMPT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PlaybackPromptWindow
{
public:
    QWidget *widget_10;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QDateTimeEdit *toDatetimeInput;
    QDateTimeEdit *fromDatetimeInput;
    QPushButton *confirmButton;
    QLineEdit *frameIndexInput;

    void setupUi(QWidget *PlaybackPromptWindow)
    {
        if (PlaybackPromptWindow->objectName().isEmpty())
            PlaybackPromptWindow->setObjectName("PlaybackPromptWindow");
        PlaybackPromptWindow->resize(271, 381);
        widget_10 = new QWidget(PlaybackPromptWindow);
        widget_10->setObjectName("widget_10");
        widget_10->setGeometry(QRect(10, 10, 251, 361));
        widget_10->setStyleSheet(QString::fromUtf8("background-color: rgb(59, 22, 22);"));
        label_9 = new QLabel(widget_10);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(20, 20, 121, 21));
        QFont font;
        font.setPointSize(12);
        label_9->setFont(font);
        label_9->setStyleSheet(QString::fromUtf8("color: rgb(171, 141, 21);"));
        label_10 = new QLabel(widget_10);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(20, 110, 121, 21));
        label_10->setFont(font);
        label_10->setStyleSheet(QString::fromUtf8("color: rgb(171, 141, 21);"));
        label_11 = new QLabel(widget_10);
        label_11->setObjectName("label_11");
        label_11->setGeometry(QRect(20, 190, 121, 21));
        label_11->setFont(font);
        label_11->setStyleSheet(QString::fromUtf8("color: rgb(171, 141, 21);"));
        toDatetimeInput = new QDateTimeEdit(widget_10);
        toDatetimeInput->setObjectName("toDatetimeInput");
        toDatetimeInput->setGeometry(QRect(20, 220, 211, 35));
        toDatetimeInput->setFont(font);
        toDatetimeInput->setStyleSheet(QString::fromUtf8("background-color: rgb(74, 74, 74);"));
        fromDatetimeInput = new QDateTimeEdit(widget_10);
        fromDatetimeInput->setObjectName("fromDatetimeInput");
        fromDatetimeInput->setGeometry(QRect(20, 140, 211, 35));
        fromDatetimeInput->setFont(font);
        fromDatetimeInput->setStyleSheet(QString::fromUtf8("background-color: rgb(74, 74, 74);"));
        confirmButton = new QPushButton(widget_10);
        confirmButton->setObjectName("confirmButton");
        confirmButton->setGeometry(QRect(20, 290, 211, 51));
        QFont font1;
        font1.setPointSize(24);
        confirmButton->setFont(font1);
        confirmButton->setStyleSheet(QString::fromUtf8("background-color: rgb(171, 141, 21);"));
        frameIndexInput = new QLineEdit(widget_10);
        frameIndexInput->setObjectName("frameIndexInput");
        frameIndexInput->setGeometry(QRect(20, 50, 211, 35));
        frameIndexInput->setFont(font);
        frameIndexInput->setStyleSheet(QString::fromUtf8("background-color: rgb(74, 74, 74);"));

        retranslateUi(PlaybackPromptWindow);

        QMetaObject::connectSlotsByName(PlaybackPromptWindow);
    } // setupUi

    void retranslateUi(QWidget *PlaybackPromptWindow)
    {
        PlaybackPromptWindow->setWindowTitle(QCoreApplication::translate("PlaybackPromptWindow", "Playback", nullptr));
        label_9->setText(QCoreApplication::translate("PlaybackPromptWindow", "Choose Frame:", nullptr));
        label_10->setText(QCoreApplication::translate("PlaybackPromptWindow", "From:", nullptr));
        label_11->setText(QCoreApplication::translate("PlaybackPromptWindow", "To:", nullptr));
        toDatetimeInput->setDisplayFormat(QCoreApplication::translate("PlaybackPromptWindow", "dd/MM/yyyy hh:mm", nullptr));
        fromDatetimeInput->setDisplayFormat(QCoreApplication::translate("PlaybackPromptWindow", "dd/MM/yyyy hh:mm", nullptr));
        confirmButton->setText(QCoreApplication::translate("PlaybackPromptWindow", "CONFIRM", nullptr));
        frameIndexInput->setText(QCoreApplication::translate("PlaybackPromptWindow", "1", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PlaybackPromptWindow: public Ui_PlaybackPromptWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLAYBACKPROMPT_H

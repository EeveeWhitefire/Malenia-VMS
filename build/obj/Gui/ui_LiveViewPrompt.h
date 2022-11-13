/********************************************************************************
** Form generated from reading UI file 'LiveViewPrompt.ui'
**
** Created by: Qt User Interface Compiler version 6.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LIVEVIEWPROMPT_H
#define UI_LIVEVIEWPROMPT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LiveViewPromptWindow
{
public:
    QWidget *widget_10;
    QLabel *label_9;
    QPushButton *confirmButton;
    QLineEdit *frameIndexInput;

    void setupUi(QWidget *LiveViewPromptWindow)
    {
        if (LiveViewPromptWindow->objectName().isEmpty())
            LiveViewPromptWindow->setObjectName("LiveViewPromptWindow");
        LiveViewPromptWindow->resize(271, 211);
        widget_10 = new QWidget(LiveViewPromptWindow);
        widget_10->setObjectName("widget_10");
        widget_10->setGeometry(QRect(10, 10, 251, 191));
        widget_10->setStyleSheet(QString::fromUtf8("background-color: rgb(59, 22, 22);"));
        label_9 = new QLabel(widget_10);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(20, 20, 121, 21));
        QFont font;
        font.setPointSize(12);
        label_9->setFont(font);
        label_9->setStyleSheet(QString::fromUtf8("color: rgb(171, 141, 21);"));
        confirmButton = new QPushButton(widget_10);
        confirmButton->setObjectName("confirmButton");
        confirmButton->setGeometry(QRect(20, 120, 211, 51));
        QFont font1;
        font1.setPointSize(24);
        confirmButton->setFont(font1);
        confirmButton->setStyleSheet(QString::fromUtf8("background-color: rgb(171, 141, 21);"));
        frameIndexInput = new QLineEdit(widget_10);
        frameIndexInput->setObjectName("frameIndexInput");
        frameIndexInput->setGeometry(QRect(20, 50, 211, 35));
        frameIndexInput->setFont(font);
        frameIndexInput->setStyleSheet(QString::fromUtf8("background-color: rgb(74, 74, 74);"));

        retranslateUi(LiveViewPromptWindow);

        QMetaObject::connectSlotsByName(LiveViewPromptWindow);
    } // setupUi

    void retranslateUi(QWidget *LiveViewPromptWindow)
    {
        LiveViewPromptWindow->setWindowTitle(QCoreApplication::translate("LiveViewPromptWindow", "Live View", nullptr));
        label_9->setText(QCoreApplication::translate("LiveViewPromptWindow", "Choose Frame:", nullptr));
        confirmButton->setText(QCoreApplication::translate("LiveViewPromptWindow", "CONFIRM", nullptr));
        frameIndexInput->setText(QCoreApplication::translate("LiveViewPromptWindow", "1", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LiveViewPromptWindow: public Ui_LiveViewPromptWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LIVEVIEWPROMPT_H

/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QCommandLinkButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QCheckBox *teamButton;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *playerName;
    QCommandLinkButton *settingButton;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *botButton;
    QPushButton *realButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        QFont font;
        font.setPointSize(13);
        MainWindow->setFont(font);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        teamButton = new QCheckBox(centralwidget);
        teamButton->setObjectName("teamButton");
        teamButton->setGeometry(QRect(230, 110, 281, 37));
        teamButton->setMinimumSize(QSize(281, 31));
        QFont font1;
        font1.setPointSize(14);
        font1.setBold(true);
        teamButton->setFont(font1);
        teamButton->setStyleSheet(QString::fromUtf8("color: red; padding: 50px;"));
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(130, 20, 591, 42));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName("label");
        label->setMinimumSize(QSize(50, 19));
        QFont font2;
        font2.setPointSize(13);
        font2.setBold(true);
        label->setFont(font2);

        horizontalLayout->addWidget(label);

        playerName = new QLineEdit(layoutWidget);
        playerName->setObjectName("playerName");
        playerName->setMinimumSize(QSize(270, 27));

        horizontalLayout->addWidget(playerName);

        settingButton = new QCommandLinkButton(layoutWidget);
        settingButton->setObjectName("settingButton");
        settingButton->setMinimumSize(QSize(90, 40));

        horizontalLayout->addWidget(settingButton);

        layoutWidget1 = new QWidget(centralwidget);
        layoutWidget1->setObjectName("layoutWidget1");
        layoutWidget1->setGeometry(QRect(20, 190, 701, 141));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget1);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        botButton = new QPushButton(layoutWidget1);
        botButton->setObjectName("botButton");
        botButton->setMinimumSize(QSize(290, 100));
        botButton->setFont(font1);

        horizontalLayout_2->addWidget(botButton);

        realButton = new QPushButton(layoutWidget1);
        realButton->setObjectName("realButton");
        realButton->setMinimumSize(QSize(290, 100));
        realButton->setFont(font1);

        horizontalLayout_2->addWidget(realButton);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 35));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        teamButton->setText(QCoreApplication::translate("MainWindow", "Play In Team", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Name", nullptr));
        playerName->setPlaceholderText(QString());
        settingButton->setText(QCoreApplication::translate("MainWindow", "Setting", nullptr));
        botButton->setText(QCoreApplication::translate("MainWindow", "Play with Bots", nullptr));
        realButton->setText(QCoreApplication::translate("MainWindow", "Play With Real Players", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

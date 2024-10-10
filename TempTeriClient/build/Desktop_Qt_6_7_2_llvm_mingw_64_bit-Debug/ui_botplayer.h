/********************************************************************************
** Form generated from reading UI file 'botplayer.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BOTPLAYER_H
#define UI_BOTPLAYER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_botPlayer
{
public:

    void setupUi(QDialog *botPlayer)
    {
        if (botPlayer->objectName().isEmpty())
            botPlayer->setObjectName("botPlayer");
        botPlayer->resize(400, 300);

        retranslateUi(botPlayer);

        QMetaObject::connectSlotsByName(botPlayer);
    } // setupUi

    void retranslateUi(QDialog *botPlayer)
    {
        botPlayer->setWindowTitle(QCoreApplication::translate("botPlayer", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class botPlayer: public Ui_botPlayer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BOTPLAYER_H

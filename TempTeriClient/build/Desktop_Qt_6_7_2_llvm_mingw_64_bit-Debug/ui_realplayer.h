/********************************************************************************
** Form generated from reading UI file 'realplayer.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REALPLAYER_H
#define UI_REALPLAYER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_realPlayer
{
public:

    void setupUi(QDialog *realPlayer)
    {
        if (realPlayer->objectName().isEmpty())
            realPlayer->setObjectName("realPlayer");
        realPlayer->resize(1067, 544);

        retranslateUi(realPlayer);

        QMetaObject::connectSlotsByName(realPlayer);
    } // setupUi

    void retranslateUi(QDialog *realPlayer)
    {
        realPlayer->setWindowTitle(QCoreApplication::translate("realPlayer", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class realPlayer: public Ui_realPlayer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REALPLAYER_H

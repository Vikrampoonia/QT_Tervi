#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "realplayer.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->playerName->setPlaceholderText("Enter Your Name");
    playTeam=false;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_settingButton_clicked()
{
   // qDebug()<<"Setting button is Clicked now you go to setting menu";

}


void MainWindow::on_teamButton_stateChanged(int arg1)
{
    //qDebug()<<"Value of arg1: "<<arg1;

    playTeam=true;

}


void MainWindow::on_botButton_clicked()
{
    //qDebug()<<"Bot Button Clicked";
    QString str=ui->playerName->text();
    if(!str.isEmpty())
    {
        // here bot logic is there
       /* botPlayer* playerWindow=new botPlayer(this);
        playerWindow->setModal(false);
        playerWindow->setWindowTitle(str);
        playerWindow->show();*/
    }
    else
    {
        //show message in QMessageBox regarding fill their name
        QMessageBox::information(this,"Player name","First Enter Your name");
    }
    // Move to another window where only bots play game
}



void MainWindow::on_realButton_clicked()
{
    //qDebug()<<"Real player button clicked";
    QString str=ui->playerName->text();
    if(!str.isEmpty())
    {
        // change variable state to true
        //close this window and open another window
        realPlayer* playerWindow=new realPlayer(this);
        playerWindow->setModal(false);
        playerWindow->setWindowTitle(str);
        playerWindow->show();
        //this->close();
       // playerWindow->setFocusPolicy(Qt::StrongFocus);
    }
    else
    {
        //show message in QMessageBox regarding fill their name
        QMessageBox::information(this,"Player name","First Enter Your name");
    }
    //Move to another window where real players play game
}

QString MainWindow::getPlayerName()
{
   // qDebug()<<"Return player name: "<<ui->playerName->text();
    return ui->playerName->text();
}

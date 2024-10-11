#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    server(new QTcpServer(this))
{
    ui->setupUi(this);

    // Start the server
    connect(server, &QTcpServer::newConnection, this, &MainWindow::onNewConnection);
    if (server->listen(QHostAddress::Any, 1234)) {
        qDebug() << "Server started on port 1234";
    } else {
        qDebug() << "Server failed to start: " << server->errorString();
    }

    //intialise one room for both team and individual
    initialiseRoom(0);
    initialiseRoom(1);

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onNewConnection()
{
    clientSocket = server->nextPendingConnection();
    connect(clientSocket, &QTcpSocket::readyRead, this, &MainWindow::onReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &MainWindow::onClientDisconnected);
    qDebug() << "Client connected!";
}

void MainWindow::onReadyRead()
{
    allocateRoom();
}


void MainWindow::allocateRoom()
{

    QByteArray data = clientSocket->readAll();
    QString str=QString::fromUtf8(data);
    qDebug()<<"Message sent by client"<<str;
    if(clientAssociated.find(clientSocket)!=clientAssociated.end())
    {
        //connected+"l"+room number+"l";
        int roomNumber,connectionOrder;
        if(str==false)
        {
            int size=roomLocator.size();
            if(roomLocator[size-1]->size()==4)
            {
                //need to intialise room
                initialiseRoom(0);
            }
            roomLocator[roomLocator.size()-1]->clientId.push_back(clientSocket);
            clientAssociated[clientSocket]=QString::number(roomLocator.size()-1)+"f";
            roomNumber=roomLocator.size()-1;
            connectionOrder=roomLocator[roomLocator.size()-1]->clientId.size()-1;
        }
        else
        {
            int size=roomLocatorTeam.size();
            if(roomLocatorTeam[size-1]->clientId.size()==4)
            {
                //need to intialise room
                initialiseRoom(1);
            }
            roomLocatorTeam[size-1]->clientId.push_back(clientSocket);
            clientAssociated[clientSocket]=QString::number(roomLocatorTeam.size()-1)+"t";
            roomNumber=roomLocatorTeam.size()-1;
            connectionOrder=roomLocatorTeam[roomLocatorTeam.size()-1]->clientId.size()-1;
        }

        qDebug()<<"Room associated with this client is: "<<clientAssociated[clientSocket];
        qDebug()<<"Message sent to client"<<QString::number(roomNumber)+QString::number(connectionOrder);
        clientSocket->write((QString::number(roomNumber)+QString::number(connectionOrder)).toUtf8());
    }
    else
    {
        findAction();
    }
}

void MainWindow::findAction()
{
    Rooms* room;
    QString str=clientAssociated[clientSocket];
    QString length1="";

    for(int i=0; i<str.size()-1; i++)
    {
        length1+=str[i];
    }
    bool ok;
    int roomNum=length1.toInt(&ok);

    if(ok)
    {
        if(str[str.size()-1]=='t')
        {
            room=roomLocatorTeam[roomNum];
        }
        else
        {
            room=roomLocator[roomNum];
        }

        room->setStartMove();
        int move=room->getStartMove();
        str=QString::fromUtf8(clientSocket->readAll());

        if(room->getServerMove)
        {
            //start 13 card game
            actionAfterStart(room,move,str);
        }
        else
        {
            //start basic info before game start
            actionBeforeStart(room,move,str);
        }
    }
    else
    {
        qDebug()<<"Conversion failed";
    }


}

void MainWindow::actionBeforeStart(Rooms* room, int move, QString str)
{
    if((move-1)/4==0)
    {
        //QString::number(name.length())+"l"+m->getPlayerName()+"True":"False";

        for(int i=0; i<str.size(); i++)
        {

        }




    }
}





void MainWindow::onClientDisconnected()
{
    qDebug() << "Client disconnected";
    clientSocket->deleteLater();
}


void MainWindow::initialiseRoom(int flag)
{
    Rooms* room=new Rooms();
    if(flag)
    {
        roomLocatorTeam.push_back(room);
    }
    else
    {
        roomLocator.push_back(room);
    }
}


Rooms::Rooms()
{
    serverMove=false;
    startMove=0;
    currentPlayer=0;
    currentRound=0;
}

bool Rooms::getServerMove()
{
    return serverMove;
}

void Rooms::setServerMove()
{
    serverMove=!serverMove;
}

int Rooms::getStartMove()
{
    return startMove;
}

void Rooms::setStartMove()
{
    startMove++;
}

int Rooms::getCurrentRound()
{
    return currentRound;
}

void Rooms::setCurrentRound()
{
    currentRound++;
}



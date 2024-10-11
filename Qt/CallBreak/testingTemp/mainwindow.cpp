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
    //qDebug()<<"Welcome to allocate room";
    QByteArray data = clientSocket->readAll();
    QString str=QString::fromUtf8(data);
    qDebug()<<"Message sent by client"<<str;
    if(clientAssociated.find(clientSocket)==clientAssociated.end())
    {
        //connected+"l"+room number+"l";
        int roomNumber,connectionOrder;
        if(str=="false")
        {
            int size=roomLocator.size();
            if(roomLocator[size-1]->clientId.size()==4)
            {
                //need to intialise room
                initialiseRoom(0);
            }
            roomLocator[roomLocator.size()-1]->clientId.push_back(clientSocket);
            clientAssociated[clientSocket]=QString::number(roomLocator.size()-1)+"f";
            roomNumber=roomLocator.size()-1;
            roomLocator[roomLocator.size()-1]->setTeam(false);
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
            roomLocatorTeam[roomLocatorTeam.size()-1]->setTeam(true);
            connectionOrder=roomLocatorTeam[roomLocatorTeam.size()-1]->clientId.size()-1;
        }

       // qDebug()<<"Room associated with this client is: "<<clientAssociated[clientSocket];
        //qDebug()<<"Message sent to client"<<QString::number(roomNumber)+"l"+QString::number(connectionOrder);
        clientSocket->write((QString::number(connectionOrder)+"l"+QString::number(roomNumber)+"l").toUtf8());
    }
    else
    {
        findAction(str);
    }
}

void MainWindow::findAction(QString data)
{
    Rooms* room;
    QString str=clientAssociated[clientSocket];
    //qDebug()<<"clientAssociated[clientSocket]: "<<str;
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

        //qDebug()<<"Message sent to client: "<<data;
        if(room->getServerMove()==true)
        {
            //start 13 card game
            //actionAfterStart(room,move,str);
        }
        else
        {
            //start basic info before game start
            actionBeforeStart(room,move,data);
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
        room->playerName.push_back(str);

        if((move)%4==0)
        {
            //orderofconnection+playerName  +13cardValue+currentMove
            //qDebug()<<"ALl name: "<<room->playerName;
            QString response="";
            for(int i=0; i<4; i++)
            {
                response+=QString::number(i)+QString::number(room->playerName[i].size())+"l"+room->playerName[i];
            }
           // qDebug()<<"Response :"<<response;

            CardInfo* c=new CardInfo();


            for(int i=0; i<4; i++)
            {
                QString cardValue="";
                room->p[i]->initialiseMembers();
                room->p[i]->setCard(i,c);
                //qDebug()<<"playerNumber: "<<i;
                for(int j=0; j<13; j++)
                {
                    //qDebug()<<"Cardvalue: "<<room->p[i]->getCard(j);
                    cardValue+=QString::number(room->p[i]->getCard(j))+"l";
                }

                //qDebug()<<"Response sent to client: "<<(response+cardValue);
                room->clientId[i]->write((response+cardValue+QString::number(room->getCurrentPlayer())).toUtf8());
            }

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
    startPlayer=0;

    for(int i=0; i<4; i++)
    {
        playerCardInfo* player=new playerCardInfo();
        p.push_back(player);
    }

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


void Rooms::setTeam(bool flag)
{
    team=flag;
}


bool Rooms::getTeam()
{
    return team;
}

void Rooms::setCurrentPlayer(int val)
{
    currentPlayer=val;
}
int Rooms::getCurrentPlayer()
{
    return currentPlayer;
}
void Rooms::setStartPlayer(int val)
{
    startPlayer=val;
}
int Rooms::getStartPlayer()
{
    return startPlayer;
}

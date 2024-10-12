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
    QTcpSocket *clientSocket = server->nextPendingConnection();
    clients.append(clientSocket);  // Add the new client to the list

    connect(clientSocket, &QTcpSocket::readyRead, this, &MainWindow::onReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &MainWindow::onClientDisconnected);
    qDebug() << "Client connected!";
}

void MainWindow::onReadyRead()
{
    // Get the socket that sent the signal
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());

    if (clientSocket)
    {
        QByteArray data = clientSocket->readAll();
        QString str=QString::fromUtf8(data);

        allocateRoom(str,clientSocket);
        // You can process the message or broadcast it to other clients here
    }

}


void MainWindow::allocateRoom(QString str,QTcpSocket* clientSocket)
{
    //qDebug()<<"Welcome to allocate room";

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
        findAction(str,clientSocket);
    }
}

void MainWindow::findAction(QString data,QTcpSocket* clientSocket)
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
            actionAfterStart(room,move,data,clientSocket);
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

                qDebug()<<"Response sent to client: "<<(response+cardValue);
                room->clientId[i]->write((response+cardValue+QString::number(room->getCurrentPlayer())).toUtf8());
            }
            qDebug()<<"Successfully out from first move";

        }

    }

    else if(move==5)
    {
        //send Message of trumpColor+suggested bid+updated 13Card value
        //get trumpCOlor code

        //update all player cardValue
        int trumpColor=str[0].unicode()-'0';
        for(int i=0; i<4; i++)
        {
            if(trumpColor!=3)
            {
                int start=room->p[i]->get_Initial_And_FinalIndex(3, 0);
                int end=room->p[i]->get_Initial_And_FinalIndex(3 , 1 );
                int value=0;
                //qDebug()<<"start: "<<start<<"  end: "<<end;
                for(int j=start; j<=end; j++)
                {

                    value=(room->p[i]->getCard(j)-((3-trumpColor)*13));
                    room->p[i]->setCard(j,value);

                }

                //trump color->spade color
                start=room->p[i]->get_Initial_And_FinalIndex( trumpColor , 0);
                end=room->p[i]->get_Initial_And_FinalIndex(trumpColor, 1 );

                for(int j=start; j<=end; j++)
                {

                    value=(room->p[i]->getCard(j)+((3-trumpColor)*13));
                    room->p[i]->setCard(j,value);
                }


                room->p[i]->sortCard();
                room->p[i]->set_Initial_And_FinalIndex();
            }

            QString response="";
            response+=QString::number(trumpColor)+QString::number(room->p[i]->suggestionSet())+"l";
            QString cardValue="";
            for(int j=0; j<13; j++)
            {
                cardValue+=QString::number(room->p[i]->getCard(j))+"l";
            }
            //qDebug()<<"in 5: response sent: "<<(response+cardValue);
            room->clientId[i]->write((response+cardValue).toUtf8());
        }


    }

    else if(move<10)
    {
        //(client number+number of bid)
        int indx=str[0].unicode()-'0';
        QString length1="";

        for(int i=1; i<str.size(); i++)
        {
            length1+=str[i];
        }

        bool ok;
        int bid=length1.toInt(&ok);
        if(ok)
        {
            if(room->getTeam())
            {
                if((room->getStartPlayer())==indx || (room->getStartPlayer()+2)%4==indx)
                {
                    bid=8;
                }
                else
                {
                    bid=6;
                }
            }
            room->p[indx]->setScoreCard(bid);
        }
        else
        {
            qDebug()<<"Conversion failed";
        }

        if(move==9)
        {
            //bid+l  first player number
            QString response="";
            for(int i=0; i<4; i++)
            {
                response+=QString::number(room->p[i]->set)+"l";
            }
            response+=QString::number(room->getCurrentPlayer());
            for(int i=0; i<4; i++)
            {
                room->clientId[i]->write(response.toUtf8());
            }
            room->setServerMove();
            room->setStartMove(0);
        }

    }

}

void MainWindow::actionAfterStart(Rooms* room, int move, QString str,QTcpSocket* clientSocket)
{
    //whomSent+cardValue
    qDebug()<<"Welcome to game sart";
    if(str=="OK SERVER")
    {
        //one round is complete send score

        /*if(room->getTeam())
        {

        }*/
        //else
        //{
            QString response="";
            for(int i=0; i<4; i++)
            {
                qDebug()<<room->p[i]->getScoreCard(room->getCurrentRound());
                response+=QString::number(room->p[i]->getScoreCard(room->getCurrentRound()))+"l";
            }
            qDebug()<<"Response"<<response;

                clientSocket->write(response.toUtf8());


            qDebug()<<"game is end so update whole server";
        //}
            if(move%4==0)
            {
                room->setServerMove();
                room->setStartMove(0);
            }


    }
    else
    {
        int playerThrow=str[0].unicode()-'0';
        QString cardValue="";

        for(int i=1; i<str.size(); i++)
        {
            cardValue+=str[i];
        }

        bool ok;
        int value=cardValue.toInt(&ok);
        if(ok)
        {
            //set in playerCardInfo
            room->p[playerThrow]->setCard(value);
        }
        else
        {
            qDebug()<<"Invalid card conversion";
        }
        qDebug()<<"Player throw: "<<playerThrow<<" value: "<<value<<"   cardValue:"<<cardValue;

        room->cardInstance.push_back(value);
        room->setCurrentPlayer((playerThrow+1)%4);
        std::vector<int>validCard;

        if(move%4==0)
        {
            //one move is complete
            //first Send playerWhomSent+PlayerWhoThrowCard+CardValue+'l'+PlayerWhoseTurn

            for(int i=0; i<4; i++)
            {
                room->clientId[i]->write((QString::number(i)+QString::number(playerThrow)+cardValue+"l").toUtf8());
            }

            //find whom set it is
            int setter= setMadeBy(room->getCurrentPlayer(),room->cardInstance);
            qDebug()<<"Setter: "<<setter;

            room->setCurrentPlayer(setter);

            room->p[setter]->updateProgressingSet();

            if(room->getTeam())
            {
                room->p[(setter+2)%4]->updateProgressingSet();
            }

            room->cardInstance.clear();


            if((move/4)!=13)
            {
                room->clientId[setter]->write((QString::number(setter)).toUtf8());
            }

        }
        else
        {
            //first Send playerWhomSent+PlayerWhoThrowCard+CardValue+'l'


            for(int i=0; i<4; i++)
            {
                room->clientId[i]->write((QString::number(i)+QString::number(playerThrow)+cardValue+"l").toUtf8());
            }

            //nextTurn   playerWhomSent+validcard+l+nextTurn


            showValidCard(room->cardInstance, room->getCurrentPlayer(), room->cardInstance[0], room->p,validCard);
            cardValue="";

            for(int i=0; i<validCard.size(); i++)
            {
                cardValue+=QString::number(validCard[i])+"l";
            }
            validCard.clear();
            qDebug()<<"ValidCard are: "<<cardValue;
            room->clientId[room->getCurrentPlayer()]->write((cardValue+QString::number(room->getCurrentPlayer())).toUtf8());
            cardValue="";
        }
    }


}



void MainWindow::onClientDisconnected()
{

    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());

    if (clientSocket)
    {
        clients.removeOne(clientSocket);  // Remove the client from the list
        clientSocket->deleteLater();
        qDebug() << "Client disconnected";
    }

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

void Rooms::setStartMove(int val)
{
    startMove=0;
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



void showValidCard(std::vector<int>&cardInstance, int currentPlayer, int firstPlayerCardValue, std::vector<playerCardInfo*> p,std::vector<int>&validCard)
{
    //finding greatest card
    int firstPlayerCardColor=(firstPlayerCardValue-2)/13;
    int maxCard=firstPlayerCardValue;

    for(int i=0; i<cardInstance.size(); i++)
    {
        if( ( (cardInstance[i]-2)/13 == firstPlayerCardColor  ||  (cardInstance[i]-2)/13 == 3 ) &&  maxCard<cardInstance[i])
        {
            maxCard=cardInstance[i];
        }
    }


    //player have same color card as first player card color
    int noOfCard=countColorCard(currentPlayer,p,firstPlayerCardColor);

    //yes player have cards as first player card color
    if(noOfCard>0)
    {
        int initialIndex=p[currentPlayer]->get_Initial_And_FinalIndex(firstPlayerCardColor,0);
        int finalIndex=p[currentPlayer]->get_Initial_And_FinalIndex(firstPlayerCardColor,1);
        helper_of_showValidCard(initialIndex,finalIndex,maxCard, currentPlayer,p,validCard);
    }
    //no player do not have first player card color check for priority color card
    else if(countColorCard(currentPlayer,p,3)>0)
    {
        int initialIndex=p[currentPlayer]->get_Initial_And_FinalIndex(3,0);
        int finalIndex=p[currentPlayer]->get_Initial_And_FinalIndex(3,1);
        int flag=0;
        for(int i=initialIndex; i<=finalIndex; i++)
        {
            if(p[currentPlayer]->getCard(i)!=-1)
            {
                if(p[currentPlayer]->getCard(i) > maxCard)
                {
                    validCard.insert(validCard.begin(), p[currentPlayer]->getCard(i));
                    flag=1;
                }
                else if(flag==0)
                {
                    //store other color card except spade because first player card does not have and in spade do not have greater card
                    helper_of_showValidCard( currentPlayer,p,validCard);
                    break;
                }
            }
        }
    }
    //player do not have first plyaer color card as well as priority color  card so store other color card
    else
    {
        helper_of_showValidCard( currentPlayer,p,validCard);
    }

}

//help to find no of cards in a color
int countColorCard(int currentPlayer,  std::vector<playerCardInfo*> p,int cardColor)
{
    int initialIndex=p[currentPlayer]->get_Initial_And_FinalIndex(cardColor,0);
    int finalIndex=p[currentPlayer]->get_Initial_And_FinalIndex(cardColor,1);
    int count=0;

    if(initialIndex!=-1)
    {
        for(int i=initialIndex; i<=finalIndex; i++)
        {
            if(p[currentPlayer]->getCard(i)!=-1)
            {
                count++;
            }
        }
    }

    return count;
}

//help to store valid card
void helper_of_showValidCard(int initialIndex,int finalIndex,int maxCard, int currentPlayer,std::vector<playerCardInfo*> p,std::vector<int>& validCard)
{
    //if maxcard color is same as firstplayercard
    int flag=0;

    for(int i=initialIndex; i<=finalIndex; i++)
    {
        if(p[currentPlayer]->getCard(i) > maxCard && p[currentPlayer]->getCard(i)!=-1)
        {
            validCard.insert(validCard.begin(), p[currentPlayer]->getCard(i));
            flag=1;
        }
        else if(flag==0  && p[currentPlayer]->getCard(i)!=-1)
        {
            validCard.insert(validCard.begin(), p[currentPlayer]->getCard(i));
        }
    }
}

//store other color card except priority color card
void helper_of_showValidCard( int currentPlayer,std::vector<playerCardInfo*> p,std::vector<int>& validCard)
{
    for(int i=0; i<13; i++)
    {
        if(p[currentPlayer]->getCard(i)!=-1)
        {
            validCard.insert(validCard.begin(), p[currentPlayer]->getCard(i));
        }
    }
}

//identify whom set is
int setMadeBy(int firstPlayer,std::vector<int>&cardInstance)
{
    int cardColor=(cardInstance[0]-2)/13;

    int seter=firstPlayer;
    int max_card=-1;

    for(int i=0; i<4; i++)
    {
        int setPlayer=(firstPlayer+i)%4;
        int card_type=(cardInstance[i]-2)/13;

        if(card_type== cardColor || card_type==3)
        {
            if(max_card<cardInstance[i])
            {
                seter=setPlayer;
                max_card=cardInstance[i];
            }
        }

    }
    return seter;
}








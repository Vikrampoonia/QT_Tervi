#include "realplayer.h"
#include "ui_realplayer.h"
#include <QPair>
#include <cmath>

std::vector<QString>Images{":/img/img/2C.png",":/img/img/3C.png",":/img/img/4C.png",":/img/img/5C.png",":/img/img/6C.png",":/img/img/7C.png",":/img/img/8C.png",":/img/img/9C.png",":/img/img/10C.png",":/img/img/JC.png",":/img/img/QC.png",":/img/img/KC.png",":/img/img/AC.png",":/img/img/2D.png",":/img/img/3D.png",":/img/img/4D.png",":/img/img/5D.png",":/img/img/6D.png",":/img/img/7D.png",":/img/img/8D.png",":/img/img/9D.png",":/img/img/10D.png",":/img/img/JD.png",":/img/img/QD.png",":/img/img/KD.png",":/img/img/AD.png",":/img/img/2H.png",":/img/img/3H.png",":/img/img/4H.png",":/img/img/5H.png",":/img/img/6H.png",":/img/img/7H.png",":/img/img/8H.png",":/img/img/9H.png",":/img/img/10H.png",":/img/img/JH.png",":/img/img/QH.png",":/img/img/KH.png",":/img/img/AH.png",":/img/img/2S.png",":/img/img/3S.png",":/img/img/4S.png",":/img/img/5S.png",":/img/img/6S.png",":/img/img/7S.png",":/img/img/8S.png",":/img/img/9S.png",":/img/img/10S.png",":/img/img/JS.png",":/img/img/QS.png",":/img/img/KS.png",":/img/img/AS.png"};
std::vector<QString>Image{":/img/img/red_back.png"};
//int clientMoveMiddleLayer,currentClient;

//ISSUE CHECK PLAYER CARDS AFTER UPDATE BOTH IN CLIENT SIDE AND SERVER SIDE BEFORE AND AFTER OTHER THAT IT WORK PROPERLY


std::vector<realPlayer* >currentInstance;//individual
std::vector<realPlayer* >currentInstanceTeam; //team
realPlayer::realPlayer(MainWindow *mainWindow,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::realPlayer),m(mainWindow)
{
    qDebug()<<"Hello from starting to ending";
    ui->setupUi(this);
    m->hide();
    clientMove=0;
    trumpColor="3";
    img.clear();
    clientRound=0;

    socket = new QTcpSocket(this);  // Initialize the socket
    //qDebug()<<"Starting way";
    // Connect to the server
    connect(socket, &QTcpSocket::connected, this, &realPlayer::onConnected);
    //qDebug()<<"After connected connection";
    connect(socket, &QTcpSocket::readyRead, this, &realPlayer::onReadyRead);


    socket->connectToHost("127.0.0.1", 1234);
    qDebug()<<"After all COnnection: ";




}

realPlayer::~realPlayer()
{
    delete ui;
}


//here we prepare

void realPlayer::onReadyRead()
{
    //qDebug()<<"Welcome to readyRead";
    QByteArray data = socket->readAll();
    clientMove++;
    qDebug() << "Received from server: " << data;
    qDebug()<<"Move: "<<clientMove;
    sendResponse(data);
    data.clear();
}

void realPlayer::onConnected()
{
    qDebug() << "Connected to server!";
    //send info like your name,play in team or not


    QVBoxLayout* mainLayer=new QVBoxLayout();
    mainLayer=makeGameGui();

    //storing mainLayer pointer so that we can traverse through whole GUI
    mainLayout=mainLayer;

    this->setLayout(mainLayer);


    for(int i=0; i<3; i++)
    {

        playerNameInfo[i][0]->setText("Player"+QString::number(i));

        playerNameInfo[i][2]->setText("0/0");


        for(int j=0; j<13; j++)
        {
            QPixmap img1(Image[0]);

            playerCardLinkInfo[i][j]->setFixedSize(25, 10);

            // Scale the image to fit the QLabel while maintaining aspect ratio
            QPixmap scaledPixmap = img1.scaled(playerCardLinkInfo[i][j]->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

            // Enable scaling within the QLabel
            playerCardLinkInfo[i][j]->setScaledContents(true);

            // Set the scaled pixmap to the QLabel
            playerCardLinkInfo[i][j]->setPixmap(scaledPixmap);

            playerCardLinkInfo[i][j]->setVisible(true);
        }
    }

    //qDebug()<<"Checking now lable values for real players: ";

    realPlayerNameInfo[0]->setText("Player4");
    realPlayerNameInfo[3]->setText("0/0");

    for(int j=0; j<13; j++)
    {
        QPixmap img1(Image[0]);

        realPlayerCardInfo[j]->setFixedSize(30, 40);
        realPlayerCardInfo[j]->setRealPlayerInstance(this);

        // Scale the image to fit the QLabel while maintaining aspect ratio
        QPixmap scaledPixmap = img1.scaled(realPlayerCardInfo[j]->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

        // Enable scaling within the QLabel
        realPlayerCardInfo[j]->setScaledContents(true);

        // Set the scaled pixmap to the QLabel
        realPlayerCardInfo[j]->setPixmap(scaledPixmap);

        realPlayerCardInfo[j]->setAssociatedValue(QString::number(5));

        realPlayerCardInfo[j]->setVisible(true);
    }
    if(m->playTeam==true)
    {
        currentInstanceTeam.push_back(this);
        qDebug()<<"teamInstance: "<<this;
        socket->write("true");
    }
    else
    {
        currentInstance.push_back(this);
        qDebug()<<"teamInstance: "<<this;
        socket->write("false");
    }
}


void realPlayer::sendResponse(QByteArray data)
{
    QString str=QString::fromUtf8(data);
    int move=clientMove; qDebug()<<"move: "<<move;

    if(move==1)
    {
        //connectedNumber+"l"+room number+"l"
        QString length1="";  int flag=0;
        for(int i=0; i<str.size(); i++)
        {
            if(str[i]=='l')
            {
                qDebug()<<"length1: "<<length1;
                bool ok;
                int number = length1.toInt(&ok);
                qDebug()<<"length1: "<<length1<<"   number: "<<number;
                if (ok)
                {
                    (flag==0)?clientNumber=number:roomNumber=number;

                    flag=1;
                    length1="";

                } else
                {
                    // Conversion failed
                    qDebug() << "Conversion failed!";
                }
            }
            else
            {
                length1+=str[i];
            }
        }
        qDebug()<<"Connected: "<<clientNumber<<"   room number: "<<roomNumber;
        setRoomNumberToClickCard();

        //send your name and want to play in team or not

        QString name=m->getPlayerName();
        //name=QString::number(name.length())+"l"+name;
        qDebug()<<"name: "<<name;
        Team=m->playTeam;
        socket->write(name.toUtf8());

    }
    else if(move==2)
    {
        //client get all player name and order of connecting to  and send desired trumpCard color to server

        //now we should have to update these values into labels
        putDataIntoLabels(str);
//nameString=str;
        //response send from click lable function move2

    }
    else if(move==3)
    {
        //getting trumpColor from server inform players usinf mesageBox regarding its trumpCOlor

        //update trumpColor value;
        trumpColor=str[0];

        //update image and its card value acc to that
        updateCardImage(str);

        //put trumpColor image to small box
        bool ok;
        QPixmap img1(Images[(13*(trumpColor.toInt(&ok)+1)-1)]);
        for(int i=0; i<3; i++)
        {
            playerNameInfo[i][1]->setFixedSize(10, 15);

            QPixmap scaledPixmap = img1.scaled(playerNameInfo[i][1]->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

            playerNameInfo[i][1]->setScaledContents(true);

            playerNameInfo[i][1]->setPixmap(scaledPixmap);

            playerNameInfo[i][1]->setVisible(true);
        }

        realPlayerNameInfo[1]->setFixedSize(10, 15);

        QPixmap scaledPixmap = img1.scaled(realPlayerNameInfo[1]->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

        realPlayerNameInfo[1]->setScaledContents(true);

        realPlayerNameInfo[1]->setPixmap(scaledPixmap);

        realPlayerNameInfo[1]->setVisible(true);

        QString num=""; int temp=0;
        (str[2]=='l')?temp=1:temp=2;

        for(int i=1; i<=temp; i++)
        {
            num+=str[1];
        }

        //qDebug()<<"Suggested bid "<<num;
        //make bid gui
        if(Team==true)
        {
            num="";
            num+=QString::number(clientNumber)+"8";
            qDebug()<<"num: "<<num;
            socket->write(num.toUtf8());
        }
        else
        {
            makeBidGui(1,num);
        }


        //response send from slot onButtonClick as number of bid
    }
    else if(move==4)
    {
        //here we update all player bid to text
        putDataIntoLabels(str);

        //make grid layout
        makeGridLayout();

        //show  current player that its your turn
        int num = str[str.size() - 1].unicode() - '0';
        //currentClient=num;
        if(clientNumber==num)
        {
            realPlayerNameInfo[2]->setText("Your Turn");
            changeStateOfClickEvent(true);
        }
        //response from click slot with clickalble
    }
    else if(move<57)
    {
        qDebug()<<"Server message: "<<str;

        //currentPlayer+playerwhoThrow+throwCardValue+"l"

        //player who throw card now
        //currentPlayer+playerwhoThrow+throwCardValue+"l"+validCard+"l"+playerWhomTurnNow
        //for first firstPlayer currentPlayer+firstPlayer;

        int tempCurrentClient=str[0].unicode()-'0';
        int playerNum=str[1].unicode()-'0';
        QString cardValue="";
        int i=0;
        //qDebug()<<"str length: "<<str.size()<<" server message: "<<str;
        if(str.size()>2)
        {
            for(i=2; str[i]!='l'; i++)
            {
                cardValue+=str[i];
            }
        }
        if(str.size()!=2)
        {
            int indx=-1;

            indx=removeQlable(tempCurrentClient,playerNum,1);

            //qDebug()<<"Message from server: "<<str;
            qDebug()<<"tempCurrentClient: "<<tempCurrentClient<<"   playerThrow: "<<playerNum<<"  cardValue: "<<cardValue<<"  indx:"<<indx;


            int playerTurn=str[str.size()-1].unicode()-'0';
            //qDebug()<<"player turn:"<<playerTurn;
            if(str[str.size()-1]!='l' && str.size()!=2)
            {

                //currentPlayer+playerwhoThrow+throwCardValue+"l"+validCard+"l"+playerWhomTurnNow
                //qDebug()<<"Player turn usrd: "<<playerTurn;
                //currentClient=playerTurn;
                //tempCurrentClient=currentClient;
                tempCurrentClient=playerTurn;
                //show thrown card
                //currentInstance[(roomNumber*4+playerTurn)]->addCardTable(cardValue,indx+1);
                addCardTable(cardValue,indx+1);
                cardValue="";
                //extract valid card and make them clickable
                for(int j=i+1; j<str.size(); j++)
                {
                    if(str[j]=='l')
                    {
                        qDebug()<<"Extracted Valid cardvalue: "<<cardValue;
                        //currentInstance[(roomNumber*4+tempCurrentClient)]->changeStateOfClickEvent(true,cardValue);
                        changeStateOfClickEvent(true,cardValue);
                        cardValue="";
                    }
                    else
                    {
                        cardValue+=str[j];
                    }
                }
                //currentInstance[(roomNumber*4+tempCurrentClient)]->realPlayerNameInfo[2]->setText("Your Turn");
                realPlayerNameInfo[2]->setText("Your Turn");
            }
            else
            {
                //currentPlayer+playerwhoThrow+throwCardValue+"l"
                qDebug()<<"other player";
                //show thrown card
                if(tempCurrentClient!=playerNum)
                {
                    //currentInstance[(roomNumber*4+tempCurrentClient)]->addCardTable(cardValue,indx+1);
                    addCardTable(cardValue,indx+1);
                }
            }

            if(move%4==0)
            {
                qDebug()<<"Welcome to hide cardTable";
                qDebug()<<"Player turn usrd: "<<playerTurn;
                QTimer::singleShot(2000, this, &realPlayer::clearCardTable);
                if(tempCurrentClient==playerTurn)
                {
                    //for first firstPlayer currentPlayer+firstPlayer;
                    qDebug()<<"First Player Turn"<<playerNum;
                    //currentClient=playerTurn;
                    tempCurrentClient=playerTurn;
                    //currentInstance[(roomNumber*4+tempCurrentClient)]->realPlayerNameInfo[2]->setText("Your Turn");
                    realPlayerNameInfo[2]->setText("Your Turn");
                    //update this player bid
                    QString setText=realPlayerNameInfo[3]->text();
                    setText=updatePlayerBid(setText);
                    realPlayerNameInfo[3]->setText(setText);
                    //currentInstance[(roomNumber*4+tempCurrentClient)]->changeStateOfClickEvent(true);
                    changeStateOfClickEvent(true);
                    for(int k=0; k<4; k++)
                    {
                        if(k!=playerTurn)
                        {
                            indx=removeQlable(k,tempCurrentClient,0);
                            //setText=currentInstance[(roomNumber*4+k)]->playerNameInfo[indx][2]->text();
                            //setText=updatePlayerBid(setText);

                            if(Team)
                            {
                                currentInstanceTeam[(roomNumber*4+k)]->playerNameInfo[indx][2]->setText(setText);
                            }
                            else
                            {
                                currentInstance[(roomNumber*4+k)]->playerNameInfo[indx][2]->setText(setText);
                            }
                        }
                    }

                    if(Team)
                    {
                        currentInstanceTeam[(roomNumber*4+(tempCurrentClient+2)%4)]->realPlayerNameInfo[3]->setText(setText);
                        for(int k=0; k<4; k++)
                        {
                            if(k!=((tempCurrentClient+2)%4))
                            {
                                indx=removeQlable(k,(tempCurrentClient+2)%4,0);
                                //setText=currentInstance[(roomNumber*4+k)]->playerNameInfo[indx][2]->text();
                                //setText=updatePlayerBid(setText);
                                currentInstanceTeam[(roomNumber*4+k)]->playerNameInfo[indx][2]->setText(setText);

                            }
                        }
                        qDebug()<<"Status updated successfully";
                    }



                }


                if((move-4)/4==13)
                {
                    //one round complete
                    qDebug()<<"As one round is complete so need to initialise whole gui agian";


                    //delete gridLayout;
                    removeGridLayout();

                    //send server ok as sign of compeletion of game
                    socket->write("OK SERVER");

                }

            }


        }

        else
        {
            qDebug()<<"Invalid Message sent from server";
        }



    }
    else
    {
        //add score here
        //qDebug()<<"Welcome to check score";
        // score"l"score"l"
        std::vector<QString>v;
        QString bid="";
        for(int i=0; i<str.size(); i++)
        {
            if(str[i]!='l')
            {
                bid+=str[i];
            }
            else
            {
                //start manipulation

                v.push_back(bid);
                bid="";
            }
        }

        qDebug()<<"Bid after parsing: "<<v;
        scoreCard.pop_back();
        scoreCard.push_back(v);
        v.clear();

        prepareForNewRound();
        qDebug()<<"after prepare";
    }

    str.clear();
}


void realPlayer::putDataIntoLabels(QString str)
{   //clientMoveMiddleLayer=clientMove;
    if(clientMove==2)
    {
        std::vector<QPair<QString,QString>>ans;  //order_of_connection,Name
        int temp=0; int i=0; QString length1="";
        while(temp<4)
        {
            QString first=str[i];


            int number=0,digit=0;
            length1="";
            for(int j=i+1; str[j]!='l'; j++)
            {
                length1+=str[j];
                digit++;
            }

            bool ok;
            number = length1.toInt(&ok);
            if(ok)
            {
                QString second=str.mid(i+digit+2,number);

                ans.push_back(std::make_pair(first,second));
                i+=number+2+digit;

            }
            else
            {
                qDebug()<<"Conversion failed";
            }
            temp++;
        }


        //after extracting data now we put these values into labels
        //bottom layer clinet self
        // realPlayerNameInfo; //name,images,your turn,sets
        realPlayerNameInfo[0]->setText(QString::number(clientNumber) + ans[clientNumber].second);  //name
        //other layers layer playerNameInfo name,image,sets   top left right
        //left
        playerNameInfo[0][0]->setText( QString::number((clientNumber+1)%4)  +   ans[(clientNumber+1)%4].second);
        playerNameInfo[1][0]->setText( QString::number((clientNumber+2)%4)  +  ans[(clientNumber+2)%4].second);
        playerNameInfo[2][0]->setText( QString::number((clientNumber+3)%4)  + ans[(clientNumber+3)%4].second);

        qDebug()<<" CardValues ";

        //setting player card values now with their images
        length1="";   temp=0;
        for(int j=(i); j<str.length(); j++)
        {
            if(str[j]=='l')
            {
                qDebug()<<"Value: "<<length1;
                bool ok;
                int number1 = length1.toInt(&ok);
                //qDebug()<<"After OK";
                if(ok)
                {
                    //qDebug()<<"(number1-2): "<<(number1-2)<<"    images[(number1-2)]: "<<Images[(number1-2)];
                    QPixmap img1(Images[(number1-2)]);
                    //qDebug()<<"after img1";

                    realPlayerCardInfo[temp]->setFixedSize(40, 60);

                    // Scale the image to fit the QLabel while maintaining aspect ratio
                    QPixmap scaledPixmap = img1.scaled(realPlayerCardInfo[temp]->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                    //qDebug()<<"after scaledPixmap";
                    // Enable scaling within the QLabel
                    realPlayerCardInfo[temp]->setScaledContents(true);

                    // Set the scaled pixmap to the QLabel
                    realPlayerCardInfo[temp]->setPixmap(scaledPixmap);
                    //qDebug()<<"after setPixmap";
                    realPlayerCardInfo[temp]->setAssociatedValue(QString::number(number1));
                    //qDebug()<<"before else";
                    realPlayerCardInfo[temp]->setVisible(true);
                    temp++;
                }
                else
                {
                    qDebug()<<"Conversion failed";
                }

                length1="";
            }
            else
            {
                length1+=str[j];
            }
        }

        length1="";   length1=str[str.length()-1];
        bool ok;
        int number1 = length1.toInt(&ok);
        //updating currentCLient
        //currentClient=number1;
        qDebug()<<"After OK";
        if(ok)
        {
            realPlayer* tempPtr;
            if(this->Team)
            {
                tempPtr=currentInstanceTeam[(roomNumber*4+clientRound%4)];
            }
            else
            {
                tempPtr=currentInstance[(roomNumber*4+clientRound%4)];
            }

            //qDebug()<<"client NUmber: "<<clientNumber<<"   currententCLient"<<currentClient<<"    number1"<<number1<<"  room: "<<roomNumber<<"  (roomNumber*4+clientRound%4)"<<(roomNumber*4+clientRound%4);
            if(number1==clientNumber)
            {
                //ask for him for trumpColor
                //currentInstance[(roomNumber*4+clientRound%4)]->makeTrumpColorGUI(1);
                tempPtr->makeTrumpColorGUI(1);
                qDebug()<<"Successfully out from trumpGUI";
            }
        }
        else
        {
            qDebug()<<"Conversion failed ";
        }

        ans.clear();  length1.clear();
    }

    else if(clientMove==4)
    {
        //"4l2l6l2l0"  bid+l  first player number
        std::vector<QString>ans;
        QString bid="";
        for(int i=0; i<str.size()-1; i++)
        {
            if(str[i]=='l')
            {
                ans.push_back(bid);
                bid="";
            }
            else
            {
                bid+=str[i];
            }
        }
        int num = clientNumber;
        scoreCard.push_back(ans);
        qDebug()<<"bid vector: "<<ans;

        //left
        playerNameInfo[0][2]->setText("0/"+ans[(num+1)%4]);

        //top
        playerNameInfo[1][2]->setText("0/"+ans[(num+2)%4]);

        //right
        playerNameInfo[2][2]->setText("0/"+ans[(num+3)%4]);

        //bottom
        realPlayerNameInfo[3]->setText("0/"+ans[num]);




        ans.clear();
    }

}

void realPlayer::changeStateOfClickEvent(bool flag)
{
    for(int i=0; i < realPlayerCardInfo.size(); i++)
    {
        if(realPlayerCardInfo[i]!=nullptr)
        {
            realPlayerCardInfo[i]->setClickEvent(flag);
            realPlayerCardInfo[i]->setStyleSheet("");
        }
        else
        {
            qDebug()<<"Nullptr i: "<<i<<" size: "<<realPlayerCardInfo.size();
        }
    }
}

void realPlayer::changeStateOfClickEvent(bool flag,QString val)
{
    for(int i=0; i <realPlayerCardInfo.size(); i++)
    {
        if( realPlayerCardInfo[i]->getAssociatedValue() == val )
        {
            realPlayerCardInfo[i]->setClickEvent(flag);
            realPlayerCardInfo[i]->setStyleSheet("border: 2px solid red;");
        }
    }
}

void realPlayer::setRoomNumberToClickCard()
{
    for(int i=0; i<realPlayerCardInfo.size(); i++)
    {
        realPlayerCardInfo[i]->setRoomNumber(roomNumber);
    }
}


int realPlayer::removeQlable(int tempCurrentClient,int playerNum, int flag)
{
    int indx=-1;
    if(tempCurrentClient!=playerNum)
    {
        /*//left top right
        if(tempCurrentClient==0 && tempCurrentClient!=playerNum)
        {
            //1  2  3
            indx=playerNum-1;

        }
        else if(tempCurrentClient==1 && tempCurrentClient!=playerNum)
        {
            //2  3  0
            indx=playerNum-2;
            if(indx<0)
            {
                indx=-indx;
            }

        }
        else if(tempCurrentClient==2 && tempCurrentClient!=playerNum)
        {
            //3  0  1
            indx=playerNum-3;
            if(indx==-3)
            {
                indx=1;
            }
            if(indx==-2)
            {
                indx=2;
            }

        }
        else if(tempCurrentClient==3 && tempCurrentClient!=playerNum)
        {
            //0  1  2
            indx=playerNum;
        }
        else
        {
            qDebug()<<"Invalid tempCurrentClient from server"<<tempCurrentClient;
        }*/

        indx=playerNum-(tempCurrentClient+1)%4;
        if(indx<0)
        {
            indx+=4;
        }
        qDebug()<<"indx: "<<indx;


        QHBoxLayout* tempTopLayer=nullptr;
        QVBoxLayout* tempLeftRightLayer=nullptr;

        if(flag)
        {
            //remove one qlable from desired player except tempcurrentCLient!=playerNum
            if(indx==0)
            {
                tempLeftRightLayer=leftWidgetLayer;
            }
            else if(indx==2)
            {
                tempLeftRightLayer=rightWidgetLayer;
            }
            else if(indx==1)
            {
                tempTopLayer=topWidgetLayer;
            }

            animateCard(playerCardLinkInfo[indx][0],indx);
            if(tempLeftRightLayer)
            {
                int count =tempLeftRightLayer->count();
                if (count > 0)
                {
                    QWidget *lastLabel = tempLeftRightLayer->itemAt(count - 1)->widget();
                    tempLeftRightLayer->removeWidget(lastLabel);
                    delete lastLabel;
                }
            }
            else
            {
                int count =tempTopLayer->count();
                if (count > 0)
                {
                    QWidget *lastLabel = tempTopLayer->itemAt(count - 1)->widget();
                    tempTopLayer->removeWidget(lastLabel);
                    delete lastLabel;
                }
            }
            playerCardLinkInfo[indx].pop_back();
        }

    }
    return indx;
}

void realPlayer::animateCard(QLabel* label,int indx)
{
   /* // Create a new QPropertyAnimation object
    QPropertyAnimation *animation = new QPropertyAnimation(label, "geometry");

    // Set the duration of the animation (in milliseconds)
    animation->setDuration(200);

    // Set the starting geometry (the card's current position and size)
    QRect startRect = label->geometry();
    animation->setStartValue(startRect);

    // Set the target geometry (to simulate "coming out")
    QRect endRect = QRect(startRect.x() - 20, startRect.y() - 20, startRect.width() + 40, startRect.height() + 40);
    animation->setEndValue(endRect);

    // Set an easing curve (optional, for smoother animation)
    animation->setEasingCurve(QEasingCurve::OutQuad);

    // Start the animation
    animation->start(QAbstractAnimation::DeleteWhenStopped);
*/
}

QString realPlayer::updatePlayerBid(QString setText)
{
    QString setBefore="";    QString setAfter=""; bool flag=true;
    for(int k=0; k<setText.size(); k++)
    {
        if(flag && setText[k]!='/')
        {
            setBefore+=setText[k];
        }
        else if(!flag && setText[k]!='/')
        {
            setAfter+=setText[k];
        }
        else
        {
            flag=false;
        }
    }

    int setNum=setBefore.toInt(&flag);
    if(flag)
    {
        setNum++;
        setBefore=QString::number(setNum)+"/"+setAfter;
    }
    else
    {
        qDebug()<<"conversion failed";
    }
    //qDebug()<<"setBefore: "<<setBefore;
    return setBefore;


}

//Here we set up GUI of Game and other stuffs

QVBoxLayout* realPlayer::makeGameGui()
{
    //make game GUI
    QVBoxLayout* mainLayout=new QVBoxLayout();

    QHBoxLayout* layer=new QHBoxLayout();
    QPushButton* backButton=new QPushButton();
    backButton->setText("X");
    backButton->setFixedSize(40,30);
    QPushButton* scoreButton=new QPushButton();
    connect(scoreButton, &QPushButton::clicked, this, &realPlayer::onButtonClickedScore);


    scoreButton->setText("Score");
    scoreButton->setFixedSize(40,30);
    layer->addWidget(backButton);
    layer->addWidget(scoreButton);

    mainLayout->addLayout(layer);
    mainLayout->addLayout(makeTopLayer());
    mainLayout->addLayout(makeMiddleLayer());
    mainLayout->addLayout(makeBottomLayer());

    return mainLayout;

}

QVBoxLayout* realPlayer::makeTopLayer()
{
    //top layer construction consist of sublayer
    //1)QHBox(name+trumpImage lable)
    //2)QHBox(Card Image)
    //3)QHBox(Sets lable)

    std::vector<QLabel*>info;
    std::vector<QLabel*>info1;

    QVBoxLayout* layer=new QVBoxLayout();

    QHBoxLayout* layer1=new QHBoxLayout();
    QLabel* name=new QLabel();
    QLabel* image=new QLabel();
    name->setFixedSize(100,20);
    info.push_back(name);
    info.push_back(image);

    layer1->addWidget(name);
    layer1->addWidget(image);


    QHBoxLayout* layer2=new QHBoxLayout();
    layer2->setSpacing(-100);
    for(int i=0; i<13; i++)
    {
        QLabel* label=new QLabel();
        info1.push_back(label);

        layer2->addWidget(label);

    }
    topWidgetLayer=layer2;  //storing layer pointer to amke ease manipulation

    QHBoxLayout* layer3=new QHBoxLayout();

    QLabel* sets=new QLabel();
    sets->setFixedSize(100,20);
    info.push_back(sets);

    layer3->addWidget(sets);


    layer->addLayout(layer1);
    layer->addLayout(layer2);
    layer->addLayout(layer3);

    playerNameInfo.push_back(info);
    playerCardLinkInfo.push_back(info1);

    info.clear();
    info1.clear();
    return layer;
}

QHBoxLayout* realPlayer::makeMiddleLayer()
{
    //middlelayer(V=left+H=middle+V=right)
    QHBoxLayout* layer=new QHBoxLayout();

    //make left layer
    layer->addLayout(makeMiddleLeftLayer());

    //make middle layer
    QHBoxLayout* layer1=new QHBoxLayout();
    layer->addLayout(layer1);

    //we store this layout pointer so that we can make GUI acc to need and reduce time Complexity
    middleWindowLayer=layer1;

    //make right layer
    layer->addLayout(makeMiddleRightLayer());

    return layer;
}

QHBoxLayout* realPlayer::makeMiddleLeftLayer()
{
    std::vector<QLabel*>info;
    std::vector<QLabel*>info1;

    QHBoxLayout* layer=new QHBoxLayout();

    QVBoxLayout* layer1=new QVBoxLayout();  //name +image
    QLabel* name=new QLabel();
    QLabel* image=new QLabel();
    name->setFixedSize(70,30);

    info.push_back(name);
    info.push_back(image);

    layer1->addWidget(name);
    layer1->addWidget(image);


    QVBoxLayout* layer2=new QVBoxLayout();
    layer2->setSpacing(-50);
    for(int i=0; i<13; i++)
    {
        QLabel* label=new QLabel();

        info1.push_back(label);
        layer2->addWidget(label);

    }
    leftWidgetLayer=layer2;


    QVBoxLayout* layer3=new QVBoxLayout();
    QLabel* sets=new QLabel();
    sets->setFixedSize(70,20);
    info.push_back(sets);

    layer3->addWidget(sets);


    layer->addLayout(layer1);
    layer->addLayout(layer2);
    layer->addLayout(layer3);

    playerNameInfo.insert(playerNameInfo.begin(),info);
    playerCardLinkInfo.push_back(info1);

    info.clear();
    info1.clear();
    return layer;
}


QHBoxLayout* realPlayer::makeMiddleRightLayer()
{
    std::vector<QLabel*>info;
    std::vector<QLabel*>info1;

    QHBoxLayout* layer=new QHBoxLayout();

    QVBoxLayout* layer1=new QVBoxLayout();  //name +image

    QLabel* name=new QLabel();
    QLabel* image=new QLabel();

    name->setFixedSize(70,20);


    info.push_back(name);
    info.push_back(image);

    layer1->addWidget(name);
    layer1->addWidget(image);


    QVBoxLayout* layer2=new QVBoxLayout();
    layer2->setSpacing(-50);
    for(int i=0; i<13; i++)
    {
        QLabel* label=new QLabel();

        info1.push_back(label);
        layer2->addWidget(label);

    }
    rightWidgetLayer=layer2;

    QVBoxLayout* layer3=new QVBoxLayout();

    QLabel* sets=new QLabel();
    sets->setFixedSize(100,20);
    info.push_back(sets);
    layer3->addWidget(sets);


    layer->addLayout(layer3);
    layer->addLayout(layer2);
    layer->addLayout(layer1);

    playerNameInfo.push_back(info);
    playerCardLinkInfo.push_back(info1);

    info.clear();
    info1.clear();
    return layer;
}

QVBoxLayout* realPlayer::makeBottomLayer()
{
    //1)QHBox(name+trumpImage lable)
    //2)QHBox(Card Image)
    //3)QHBox(Sets lable)

    //realPlayerCardInfo; //clickLableCard
    //realPlayerNameInfo;

    QVBoxLayout* layer=new QVBoxLayout();

    QHBoxLayout* layer1=new QHBoxLayout();
    QLabel* name=new QLabel();
    QLabel* image=new QLabel();
    name->setFixedSize(100,30);
    image->setFixedSize(100,30);

    realPlayerNameInfo.push_back(name);
    realPlayerNameInfo.push_back(image);

    layer1->addWidget(name);
    layer1->addWidget(image);

    QHBoxLayout* layer2=new QHBoxLayout();
    layer2->setSpacing(-100);
    for(int i=0; i<13; i++)
    {
        ClickableLabel* label=new ClickableLabel();
        label->setClickEvent(false);

        realPlayerCardInfo.push_back(label);
        layer2->addWidget(label);

    }
    bottomWidgetLayer=layer2;


    QHBoxLayout* layer3=new QHBoxLayout();
    QLabel* sets=new QLabel();
    QLabel* yourTurn=new QLabel();
    sets->setFixedSize(100,30);
    yourTurn->setFixedSize(100,30);
    yourTurn->setText("YOur Turn");

    realPlayerNameInfo.push_back(sets);
    realPlayerNameInfo.push_back(yourTurn);

    layer3->addWidget(sets);
    layer3->addWidget(yourTurn);


    layer->addLayout(layer3);
    layer->addLayout(layer2);
    layer->addLayout(layer1);

    return layer;
}

void realPlayer::makeTrumpColorGUI(int flag)
{
    if(flag==1)
    {
        //only used for taking trumpColor from player
        QLabel* label=new QLabel();

        label->setText("Click On your Trump Color \n");

        QHBoxLayout* layer1=new QHBoxLayout();

        for(int i=0; i<4; i++)
        {
            ClickableLabel* label1=new ClickableLabel();
            label1->setRoomNumber(roomNumber);
            label1->setClickEvent(true);
            label1->setRealPlayerInstance(this);

            QPixmap img1(Images[(13*i)+12]);

            label1->setFixedSize(40, 60);

            // Scale the image to fit the QLabel while maintaining aspect ratio
            QPixmap scaledPixmap = img1.scaled(label1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

            // Enable scaling within the QLabel
            label1->setScaledContents(true);

            // Set the scaled pixmap to the QLabel
            label1->setPixmap(scaledPixmap);

            label1->setAssociatedValue(QString::number(i));

            label1->setVisible(true);

            layer1->addWidget(label1);

        }

        QVBoxLayout* layer=new QVBoxLayout();
        layer->addWidget(label);
        layer->addLayout(layer1);
        qDebug()<<"Middle count before adding: "<<middleWindowLayer->count();
        middleWindowLayer->addLayout(layer);

    }
    else
    {

        //QVBOX->label,QHBOX->4images


        QLayoutItem* vBoxLayout1 = middleWindowLayer->takeAt(0);
        QVBoxLayout* vBoxLayout = qobject_cast<QVBoxLayout*>(vBoxLayout1->layout());


        // Assuming 'vBoxLayout' is the pointer to your QVBoxLayout
        if (vBoxLayout != nullptr)
        {
            //qDebug()<<"number of item: "<<vBoxLayout->count();
            for(int i=vBoxLayout->count()-1; i>=1; i--)
            {
                // Remove and delete the inner QHBoxLayout and its widgets
                QLayoutItem *item = vBoxLayout->takeAt(i); // 1 if it's the second item in the vbox
                if (item != nullptr && item->layout() != nullptr)
                {
                    QLayout *innerHBoxLayout = item->layout();
                    QLayoutItem *innerItem;
                    while ((innerItem = innerHBoxLayout->takeAt(0)) != nullptr)
                    {
                        if (innerItem->widget() != nullptr)
                        {
                            delete innerItem->widget(); // delete QLabel in the inner QHBoxLayout
                        }
                        delete innerItem; // delete layout item
                    }
                    delete innerHBoxLayout; // delete the inner QHBoxLayout
                }
            }
            // Remove and delete the QLabel in the vBoxLayout (assuming it is the first widget)
            QLayoutItem *labelItem = vBoxLayout->takeAt(0); // 0 if it's the first item in the vbox
            if (labelItem != nullptr && labelItem->widget() != nullptr)
            {
                delete labelItem->widget(); // delete the QLabel
            }
            delete labelItem;

            // Now delete the vBoxLayout itself
            delete vBoxLayout;

        }



    }


}

void  realPlayer::makeBidGui(int flag, QString suggestBid)
{
    if(flag==1)
    {
        //make bid gui v+3h

        QVBoxLayout* layer=new QVBoxLayout();
        QLabel* label=new QLabel();
        label->setText("Choose No of bids");
        layer->addWidget(label);

        QHBoxLayout* layer1=new QHBoxLayout();
        QHBoxLayout* layer2=new QHBoxLayout();
        QHBoxLayout* layer3=new QHBoxLayout();

        for(int i=1; i<=13; i++)
        {
            QPushButton* button=new QPushButton();
            button->setText(QString::number(i));
            connect(button, &QPushButton::clicked, this, &realPlayer::onButtonClicked);

            if(QString::number(i)==suggestBid)
            {
                button->setStyleSheet("background-color: yellow; color: black; border: 2px solid red;");
            }
            if(i<5)
            {
                layer1->addWidget(button);
            }
            else if(i<9)
            {
                layer2->addWidget(button);
            }
            else
            {
                layer3->addWidget(button);
            }
        }

        layer->addLayout(layer1);
        layer->addLayout(layer2);
        layer->addLayout(layer3);

        middleWindowLayer->addLayout(layer);

    }

}

void realPlayer::makeGridLayout()
{

    QGridLayout* layer=new QGridLayout();

    for(int i=0; i<4; i++)
    {
        QLabel* label=new QLabel();

        int r1,c1;

        if(i==0)
        {
            r1=2,c1=1;
        }
        else if(i==1)
        {
            r1=1,c1=0;
        }
        else if(i==2)
        {
            r1=0,c1=1;
        }
        else
        {
            r1=1,c1=2;
        }

        label->setVisible(true);
        layer->addWidget(label,r1,c1);
        tableCardLabel.push_back(label);
    }

    middleWindowLayer->addLayout(layer);
}


void realPlayer::onButtonClicked()
{
    QPushButton *Button = qobject_cast<QPushButton*>(sender());

    if (Button)
    {
        QString buttonText = Button->text();  // Get the text of the clicked button

        // deleting bid gui
        makeTrumpColorGUI(0);

        makeBidGui(0,"");

        socket->write((QString::number(clientNumber)+buttonText).toUtf8());
    }
}

void realPlayer::updateCardImage(QString str)
{

        int number = str[0].unicode()-'0';

        std::vector<std::vector<std::pair<int,int>>> ArrayIndex{{std::make_pair(39,51),std::make_pair(13,25),std::make_pair(26,38),std::make_pair(0,12)},{std::make_pair(0,12),std::make_pair(39,51),std::make_pair(26,38),std::make_pair(13,25)},{std::make_pair(0,12),std::make_pair(13,25),std::make_pair(39,51),std::make_pair(26,38)},{std::make_pair(0,12),std::make_pair(13,25),std::make_pair(26,38),std::make_pair(39,51)}};

        //qDebug()<<"Before inserting img: "<<img;
        for(int i=0; i<ArrayIndex[number].size(); i++)
        {
            int first=ArrayIndex[number][i].first;
            int second=ArrayIndex[number][i].second;

            for(int j=first; j<=second; j++)
            {
                img.push_back(j);
            }

        }

            //qDebug()<<"After inserting img: "<<img;
            QString length1=""; int i=0,k=2;

            (str[2]=='l')?k=3:k=4;

                for(int j=k; j<str.length(); j++)
                {
                    if(str[j]=='l')
                    {

                        //qDebug()<<"Before update value: "<<length1;
                        bool ok1;
                        int number1=length1.toInt(&ok1);
                        if(ok1)
                        {

                            realPlayerCardInfo[i]->setAssociatedValue(QString::number(number1));

                            QPixmap img1(Images[img[(number1-2)]]);

                            //qDebug()<<"number1: "<<number1<<" img[number-2]: "<<img[(number1-2)]<<"  (Images[img[(number1-2)]]):  "<<(Images[img[(number1-2)]]);


                            realPlayerCardInfo[i]->setFixedSize(40, 60);

                            // Scale the image to fit the QLabel while maintaining aspect ratio
                            QPixmap scaledPixmap = img1.scaled(realPlayerCardInfo[i]->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

                            // Enable scaling within the QLabel
                            realPlayerCardInfo[i]->setScaledContents(true);

                            // Set the scaled pixmap to the QLabel
                            realPlayerCardInfo[i]->setPixmap(scaledPixmap);
                            realPlayerCardInfo[i++]->setVisible(true);

                            length1="";
                        }
                        else
                        {
                            qDebug()<<"Conversion failed";
                        }
                    }
                    else
                    {
                        length1+=str[j];
                    }
                }






}

void realPlayer::addCardTable(QString val, int indx)
{
    qDebug()<<"index reflect in add card table: "<<indx;
    bool ok;
    int num=val.toInt(&ok);
    if(ok)
    {

        QPixmap img1(Images[img[(num-2)]]);


        tableCardLabel[indx]->setFixedSize(40,60);

        // Scale the image to fit the QLabel while maintaining aspect ratio
        QPixmap scaledPixmap = img1.scaled(tableCardLabel[indx]->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

        // Enable scaling within the QLabel
        tableCardLabel[indx]->setScaledContents(true);

        // Set the scaled pixmap to the QLabel
        tableCardLabel[indx]->setPixmap(scaledPixmap);

        tableCardLabel[indx]->setVisible(true);
    }
    else
    {
        qDebug()<<"COnversion failed";
    }
}

void realPlayer::clearCardTable()
{
    for(int i=0; i<4; i++)
    {
        tableCardLabel[i]->clear();
    }
}

void realPlayer::prepareForNewRound()
{
    //QCoreApplication::processEvents();

    /*
        i have to add qlables in the layout and erase your turn

    */
    clientRound++;

    tableCardLabel.clear();
    playerCardLinkInfo.clear();
    realPlayerCardInfo.clear();



    //now inserting labels in vector and qlabels
    std::vector<QLabel*>info;
    for(int j=0; j<3; j++)
    {
        for(int i=0; i<13; i++)
        {
            QLabel* label=new QLabel();
            if(j==0)
            {
                leftWidgetLayer->addWidget(label);
            }
            else if(j==1)
            {
                topWidgetLayer->addWidget(label);
            }
            else
            {
                rightWidgetLayer->addWidget(label);
            }
            info.push_back(label);

        }
        playerCardLinkInfo.push_back(info);
        info.clear();
    }

    changeStateOfClickEvent(false);


    for(int j=0; j<13; j++)
    {
        ClickableLabel* label=new ClickableLabel();
        label->setRoomNumber(roomNumber);
        label->setRealPlayerInstance(this);
        realPlayerCardInfo.push_back(label);
        bottomWidgetLayer->addWidget(label);
        label->setClickEvent(false);
    }



    for (int i = 0; i < 3; i++) {
        playerNameInfo[i][2]->setText("0/0");


        for (int j = 0; j < 13; j++) {
            QPixmap img1(Image[0]);

            if (img1.isNull()) {
                qDebug() << "Failed to load image at " << Image[0];
                continue;  // Skip the current iteration if the image is invalid
            }


            playerCardLinkInfo[i][j]->setFixedSize(25, 10);


            QSize labelSize = playerCardLinkInfo[i][j]->size();
            if (labelSize.isValid() && labelSize.width() > 0 && labelSize.height() > 0) {
                QPixmap scaledPixmap = img1.scaled(labelSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);


                playerCardLinkInfo[i][j]->setScaledContents(true);


                playerCardLinkInfo[i][j]->setPixmap(scaledPixmap);  // Potential crash location

            } else {
                qDebug() << "Invalid QLabel size at playerCardLinkInfo[" << i << "][" << j << "]";
            }

            playerCardLinkInfo[i][j]->setVisible(true);

        }
    }




    realPlayerNameInfo[3]->setText("0/0");

    for(int j=0; j<13; j++)
    {

        QPixmap img1(Image[0]);

        realPlayerCardInfo[j]->setFixedSize(30, 40);

        // Scale the image to fit the QLabel while maintaining aspect ratio
        QPixmap scaledPixmap = img1.scaled(realPlayerCardInfo[j]->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

        // Enable scaling within the QLabel
        realPlayerCardInfo[j]->setScaledContents(true);

        // Set the scaled pixmap to the QLabel
        realPlayerCardInfo[j]->setPixmap(scaledPixmap);

        realPlayerCardInfo[j]->setAssociatedValue(QString::number(5));

        realPlayerCardInfo[j]->setVisible(true);

    }


    clientMove=1;
    trumpColor="3";
    img.clear();

    QString name=m->getPlayerName();
    name=QString::number(name.length())+"l"+name;
    (m->playTeam==true)?name=name+"True":name=name+"False";
    qDebug()<<"name: "<<name;
    socket->write(name.toUtf8());

    qDebug()<<"Out from new layer";


}


void realPlayer::removeGridLayout()
{
    QLayoutItem* vBoxLayout1 = middleWindowLayer->takeAt(0);
    QGridLayout* vBoxLayout = qobject_cast<QGridLayout*>(vBoxLayout1->layout());


    // Assuming 'vBoxLayout' is the pointer to your QVBoxLayout
    if (vBoxLayout != nullptr)
    {
        //qDebug()<<"number of item: "<<vBoxLayout->count();
        for(int i=vBoxLayout->count()-1; i>=0; i--)
        {
            // Remove and delete the QLabel in the vBoxLayout (assuming it is the first widget)
            QLayoutItem *labelItem = vBoxLayout->takeAt(0); // 0 if it's the first item in the vbox
            if (labelItem != nullptr && labelItem->widget() != nullptr)
            {
                delete labelItem->widget(); // delete the QLabel
            }
            delete labelItem;
        }

        // Now delete the vBoxLayout itself
        delete vBoxLayout;

    }

}



void ClickableLabel::mousePressEvent(QMouseEvent* event)
{
    if(this->getClickEvent()==true)
    {
        emit clicked();
        QLabel::mousePressEvent(event);
        QString str = this->getAssociatedValue();

        realPlayer* tempPtr;
        int currentClient=this->getRealPlayerInstance()->clientNumber;
        if(this->getRealPlayerInstance()->Team)
        {
            tempPtr=currentInstanceTeam[(this->getRoomNumber()*4+currentClient)];
        }
        else
        {
            tempPtr=currentInstance[(this->getRoomNumber()*4+currentClient)];
        }
        tempPtr=this->getRealPlayerInstance();



        if(this->getRealPlayerInstance()->clientMove==2)
        {

            tempPtr->makeTrumpColorGUI(0);
            tempPtr->getSocketValue()->write((str+"l").toUtf8());
            tempPtr->getSocketValue()->flush();

        }
        else
        {
            this->clear();

            for(int i=0; i< tempPtr->realPlayerCardInfo.size(); i++)
            {
                if( tempPtr->realPlayerCardInfo[i]==this)
                {
                    tempPtr->realPlayerCardInfo.erase(tempPtr->realPlayerCardInfo.begin()+i);
                    break;
                }
            }

            if(tempPtr->bottomWidgetLayer)
            {
                int count =tempPtr->bottomWidgetLayer->count();
                for(int j=0; j<count; j++)
                {
                    QWidget *label = tempPtr->bottomWidgetLayer->itemAt(j)->widget();

                    if(label==this)
                    {

                        tempPtr->bottomWidgetLayer->removeWidget(label);
                        label->setParent(nullptr);

                        break;
                    }
                }
            }

            tempPtr->addCardTable(str,0);

            tempPtr->changeStateOfClickEvent(false);

            QTcpSocket *socket = tempPtr->getSocketValue();
            if (socket && socket->state() == QAbstractSocket::ConnectedState) {

                qint64 bytesWritten = socket->write((QString::number(currentClient) + str).toUtf8());
                if (bytesWritten == -1) {
                    qDebug() << "Error writing to socket: " << socket->errorString();
                }

                qDebug() << "Successfully sent response to the server.";
            } else {
                qDebug() << "Socket is not connected!";
            }
            tempPtr->realPlayerNameInfo[2]->setText("");


        }

    }

}



void realPlayer::onButtonClickedScore()
{
   /* scoreBoard* playerWindow=new scoreBoard(this);
    playerWindow->setModal(true);
    //playerWindow->setWindowTitle(ScoreCard);
    playerWindow->exec();*/
}


QTcpSocket* realPlayer::getSocketValue()
{
    return socket;
}


void ClickableLabel::setClickEvent(bool flag)
{
    clickEnable=flag;
}

bool ClickableLabel::getClickEvent()
{
    return  clickEnable;
}

int ClickableLabel::getRoomNumber()
{
    return roomNumber;
}

void ClickableLabel::setRoomNumber(int val)
{
    roomNumber=val;
}

void ClickableLabel::setRealPlayerInstance(realPlayer* instance)
{
    //qDebug()<<"Setting instance: "<<instance<<"   this: "<<this<<"   instance->trumpColor"<<instance->trumpColor;
    realPlayerInstance=instance;
}

realPlayer* ClickableLabel::getRealPlayerInstance()
{
    return realPlayerInstance;
}

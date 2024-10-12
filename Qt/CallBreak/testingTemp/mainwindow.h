#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <map>
#include<vector>
#include <QList>
#include "cardinfo.h"
#include "playercardinfo.h"

namespace Ui {
class MainWindow;
}

class Rooms
{
    bool team;
    bool serverMove;
    int startMove;
    int currentPlayer;
    int startPlayer;
    int currentRound;

public:
    Rooms();
    std::vector<QTcpSocket*>clientId;
    std::vector<QString>playerName;
    std::vector<playerCardInfo*>p;
    std::vector<int>cardInstance;

    bool getServerMove();
    void setServerMove();
    int getStartMove();
    void setStartMove();
    void setStartMove(int );
    int getCurrentRound();
    void setCurrentRound();
    void setTeam(bool );
    bool getTeam();
    void setCurrentPlayer(int );
    int getCurrentPlayer();
    void setStartPlayer(int );
    int getStartPlayer();
};





class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    std::map<QTcpSocket*,QString>clientAssociated; //connect client to roomTF
    std::vector<Rooms*>roomLocator;
    std::vector<Rooms*>roomLocatorTeam;





    void initialiseRoom(int );
    void allocateRoom(QString ,QTcpSocket*);
    void findAction(QString ,QTcpSocket* );
    void actionBeforeStart(Rooms* , int , QString );
    void actionAfterStart(Rooms* , int , QString ,QTcpSocket* );

private slots:
    void onNewConnection();
    void onReadyRead();
    void onClientDisconnected();

private:
    Ui::MainWindow *ui;
    QTcpServer *server;
    QList<QTcpSocket*>clients;
};

#endif // MAINWINDOW_H

void showValidCard(std::vector<int>&, int , int , std::vector<playerCardInfo*> ,std::vector<int>&);
int countColorCard(int ,  std::vector<playerCardInfo*> ,int );
void helper_of_showValidCard(int ,int ,int , int ,std::vector<playerCardInfo*> ,std::vector<int>& );
void helper_of_showValidCard( int ,std::vector<playerCardInfo*> ,std::vector<int>& );
int setMadeBy(int ,std::vector<int>&);



//for individual
/* CLient Moves When it get data from server
 1) Get message of order of connected+"l"+room number+"l";
 2) Get Message of all  playernumber acc to they ,player name+player 13 card value+currentMove , send trumpColor by First Player
 3) Get Message of trumpColor+suggested bid+updated 13Card value ,send number of bid
 4) Get all player bid and curret player (bid+l    current move)and send (player+cardValue);
 5) Get these response from server until round over
    player who has first turn sent as playerWhomSent+PlayerWhoseTurn
    first sent all player to playerWhomSent+playernum+cardValue+"l";
    other player whom have next turn like playerWhomSent+playernum+cardValue+"l"+validCardvalue+"l"+NextTurn;

*/


//for team

/*

 2) Get Message of all  playernumber acc to they ,player name+player 13 card value+currentMove , send trumpColor and bid message by all players
 3) Get Message of trumpColor+l+suggested bid+updated 13Card value+current move
 5) Get these response from server until round over
    player who has first turn sent as playerWhomSent+PlayerWhoseTurn
    first sent all player to playerWhomSent+playernum+cardValue+"l";
    other player whom have next turn like playerWhomSent+playernum+cardValue+"l"+validCardvalue+"l"+NextTurn;
 */

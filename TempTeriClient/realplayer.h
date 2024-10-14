#ifndef REALPLAYER_H
#define REALPLAYER_H

#include <QDialog>
#include <QLabel>
#include <vector>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDebug>
#include <QTimer>
#include <QPushButton>
#include <QTcpSocket>
#include <QPair>
#include <QMessageBox>
#include <QPropertyAnimation>

#include "mainwindow.h"
#include "scoreboard.h"


namespace Ui {
class realPlayer;
}

class realPlayer;

class ClickableLabel : public QLabel
{
    Q_OBJECT

public:
    explicit ClickableLabel(QWidget* parent = nullptr)
        : QLabel(parent) {}

    void setAssociatedValue(const QString& value) {
        associatedValue = value;
        //qDebug()<<"assigned value: "<<value;
    }

    QString getAssociatedValue() const {
        return associatedValue;
    }

    void setClickEvent(bool );
    bool getClickEvent();
    int getRoomNumber();
    void setRoomNumber(int );
    void setRealPlayerInstance(realPlayer* );
    realPlayer* getRealPlayerInstance();

protected:

    void mousePressEvent(QMouseEvent* event);

signals:
    void clicked();

private:
    QString associatedValue;
    bool clickEnable;
    int roomNumber;
    realPlayer* realPlayerInstance;
};




class realPlayer : public QDialog
{
    Q_OBJECT

public:
    explicit realPlayer(MainWindow *mainWindow,QWidget *parent = nullptr);
    ~realPlayer();

    int clientMove;
    int clientNumber;
    QString  trumpColor;
    bool Team;
    //QString nameString;
    int clientRound;
    int roomNumber;

    std::vector<std::vector<QString>>scoreCard;  //store each player score
    //for each player other than real player
    // store Qlable of name and image with current/progressing set
    //store their QLable
    //these vector containing only three players info except real player
    std::vector<std::vector<QLabel*>>playerNameInfo;  //name,image,sets
    std::vector<std::vector<QLabel*>>playerCardLinkInfo; //cardLable link
    std::vector<ClickableLabel*>realPlayerCardInfo; //clickLableCard
    std::vector<QLabel*>realPlayerNameInfo; //name,images,your turn,sets
    std::vector<QLabel*>tableCardLabel; //containing table card label pointer

    //store layout pointer, reduce timeComplexity
    QVBoxLayout* mainLayout; //main pointer for layout
    QHBoxLayout* middleWindowLayer;  //middle window layer pointer

    QHBoxLayout* topWidgetLayer;
    QVBoxLayout* leftWidgetLayer;
    QVBoxLayout* rightWidgetLayer;
    QHBoxLayout* bottomWidgetLayer;



    std::vector<int>img;

    //containing game Gui
    QVBoxLayout* makeGameGui();
    QVBoxLayout* makeTopLayer();
    QHBoxLayout* makeMiddleLayer();
    QHBoxLayout* makeMiddleLeftLayer();
    QHBoxLayout* makeMiddleRightLayer();
    QVBoxLayout* makeBottomLayer();
    void makeTrumpColorGUI(int );
    void makeGridLayout();
    void  removeGridLayout();


    //containing function
    void  makeBidGui(int ,QString ); //making and deleting bid gui with its grid gui also
    void updateCardImage(QString str);
    void changeStateOfClickEvent(bool );
    void changeStateOfClickEvent(bool ,QString);
    void addCardTable(QString , int );
    void clearCardTable();
    int removeQlable(int ,int ,int );
    QString updatePlayerBid(QString );
    void prepareForNewRound();
    void setRoomNumberToClickCard();

    //about server and client side logics
    void sendResponse(QByteArray );
    void putDataIntoLabels(QString );
    QTcpSocket* getSocketValue();
    void animateCard(QLabel* ,int);


private slots:
    void onReadyRead();
    void onConnected();
    void onButtonClicked();
    void onButtonClickedScore();
private:
    Ui::realPlayer *ui;
    QTcpSocket *socket;
    MainWindow* m;
};

#endif // REALPLAYER_H

//for individual
/* CLient Moves When it get data from server
 1) Get message of order of connected+"l"+room number+"l";
 2) Get Message of all  playernumber acc to they ,player name+player 13 card value+currentMove , send trumpColor by First Player
 3) Get Message of trumpColor+suggested bid+updated 13Card value ,send (client number+number of bid)
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







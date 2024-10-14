#include "scoreboard.h"
#include "ui_scoreboard.h"
#include "realplayer.h"

scoreBoard::scoreBoard(realPlayer*  player,  QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::scoreBoard)
{
    ui->setupUi(this);
    qDebug()<<"WElcome to dialog"<<player->scoreCard;
    QVBoxLayout* mainLayout=new QVBoxLayout();
    if(player->Team)
    {
        makeScoreBoard(mainLayout,player);
    }
    else
    {
        makeScoreBoardTeam();
    }

    this->setLayout(mainLayout);

}

void scoreBoard::makeScoreBoard(QVBoxLayout* mainLayout,realPlayer* player)
{
    mainLayout->addLayout(makeFirstLayer());
    for(int i=0; i<player->scoreCard.size(); i++)
    {
        mainLayout->addLayout(makeOtherLayer(player->scoreCard[i],i));
    }
}


QHBoxLayout* scoreBoard::makeFirstLayer()
{
    QHBoxLayout* layout=new QHBoxLayout();

    for(int i=0; i<4; i++)
    {
        if(i==0)
        {
            QLabel* label=new QLabel();
            label->setText("Round");
            layout->addWidget(label);
        }
        QLabel* name=new QLabel();
        name->setText("Player"+QString::number(i+1));
    }
    return layout;
}

QHBoxLayout* makeOtherLayer(std::vector<QString> v,int num)
{
    QHBoxLayout* layout=new QHBoxLayout();



    for(int i=0; i<v.size(); i++)
    {
        if(i==0)
        {
            QLabel* round=new QLabel();
            round->setText(QString::number(num));
            layout->addWidget(round);
        }
        QLabel* score=new QLabel();
        score->setText(v[i]);
        layout->addWidget(score);
    }

    return layout;

}


scoreBoard::~scoreBoard()
{
    delete ui;
}

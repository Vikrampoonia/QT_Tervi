/*#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <QDialog>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<vector>

class realPlayer;

namespace Ui {
class scoreBoard;
}

class scoreBoard : public QDialog
{
    Q_OBJECT

public:
    explicit scoreBoard(realPlayer* player, QWidget *parent = nullptr);
    ~scoreBoard();

    void makeScoreBoard(QVBoxLayout* ,realPlayer* );
    void makeScoreBoardTeam();
    QHBoxLayout*  makeFirstLayer();
    QHBoxLayout* makeOtherLayer(std::vector<QString> ,int );

private:
    Ui::scoreBoard *ui;
};

#endif // SCOREBOARD_H
*/

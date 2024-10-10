#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QString>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool playTeam;

    QString getPlayerName();

private slots:
    void on_settingButton_clicked();

    void on_teamButton_stateChanged(int arg1);

    void on_botButton_clicked();

    void on_realButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

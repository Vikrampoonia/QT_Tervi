#include "mainwindow.h"


#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();


    // Create multiple instances of the realPlayer window
    const int numClients = 4;  // Number of client instances
    QList<MainWindow*> clientWindows;

    for (int i = 0; i < numClients; ++i) {
        MainWindow* client = new MainWindow (nullptr);  // Assuming MainWindow is optional here
        clientWindows.append(client);
        clientWindows[i]->show();
    }
    /*clientWindows[0]->show();
    clientWindows[1]->show();
    clientWindows[2]->show();
    clientWindows[3]->show();*/

    return a.exec();
}

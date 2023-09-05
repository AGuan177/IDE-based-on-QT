#include "mainwindow.h"
#include "QTimer"
#include <QApplication>
#include <QObject>
int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
    MainWindow* w = new MainWindow();
    w->show();
    QTimer *timi = new QTimer();
    timi->setInterval(50);
    timi->start();
    QObject::connect(timi, &QTimer::timeout,w,&MainWindow::op);
    app.exec();
    return 0;
}

#include "mainwindow.h"
#include "background.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BackGround w;
    w.show();
    return a.exec();
}

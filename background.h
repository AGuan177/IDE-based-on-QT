#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <QMainWindow>
#include <tree.h>
#include <mainwindow.h>

namespace Ui {
class BackGround;
}

class BackGround : public QMainWindow
{
    Q_OBJECT

public:
    explicit BackGround(QWidget *parent = 0);
    ~BackGround();

private:
    Ui::BackGround *ui;
    MainWindow * main1;
    Tree * tree1;
};

#endif // BACKGROUND_H

#include "background.h"
#include "ui_background.h"
#include<QHBoxLayout>
BackGround::BackGround(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BackGround)
{
    ui->setupUi(this);
    main1=new MainWindow();
    tree1=new Tree();
    QHBoxLayout *layout1=new QHBoxLayout();
    QHBoxLayout *layout2=new QHBoxLayout();
    layout1->addWidget(main1);
    layout2->addWidget(tree1);
    ui->widget1->setLayout(layout2);
    ui->widget2->setLayout(layout1);
}

BackGround::~BackGround()
{
    delete ui;
}

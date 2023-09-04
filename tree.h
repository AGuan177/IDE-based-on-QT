#ifndef TREE_H
#define TREE_H

#include <QMainWindow>
#include"QTreeWidgetItem"
#include"QFileInfoList"
#include"QTranslator"

QT_BEGIN_NAMESPACE
namespace Ui { class Tree; }
QT_END_NAMESPACE

class Tree : public QMainWindow
{
    Q_OBJECT

public:
    Tree(QWidget *parent = nullptr);
    ~Tree();

private:
    Ui::Tree *ui;
    QFileInfoList allfile(QTreeWidgetItem *root,QString path);

};
#endif // MAINWINDOW_H


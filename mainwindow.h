#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTableWidget>
#include <QProcess>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qsciapis.h>
#include <Qsci/qscilexercpp.h>
#include <QMainWindow>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QLabel>
#include <QString>
#include <QComboBox>
#include <QToolButton>
#include <QFontComboBox>
#include <QVector>
#include <QToolBar>
#include <QStatusBar>
#include <QDebug>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QTimer>
#include <QCoreApplication>
#include <QGraphicsView>
#include "QTreeWidgetItem"
#include <QToolBar>
#include "QFileInfoList"
#include <QIcon>
#include <QDockWidget>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QPushButton>
#include <QTextCursor>
#include <QClipboard>
#include <QLineEdit>
#include <QDialog>
#include <QVBoxLayout>
#include <QTextCharFormat>
#include <QFontDialog>
#include <QTextCodec>
#include <QSplitter>
#include <QFileInfo>
#include <QtGui/QGuiApplication>
#include <QTableWidget>
#include <QGraphicsTextItem>
#include <QInputDialog>
#include <QUrl>
#include <QDesktopServices>
#include <QSvgGenerator>
#include <QTableWidget>
#include <QHeaderView>
#include <QScrollBar>
#include "tree.h"
#include "keywords.h"
#include "Themes.h"
#include "cppcoderunner.h"
#include "cppcompileinfo.h"


class QLineEdit;
class QDialog;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //成员变量
    QAction* newfile;
    QAction* openfile;
    QAction* savefile;
    QAction* saveasfile;
    QAction* copyText;
    QAction* cutText;
    QAction* pasteText;
    QAction* seekText;
    QAction* fontSet;
    QAction* compilefile;
    QAction* compileAndRun;
    QAction* runfile;
    QAction* undoe;
    QAction* redoe;
    QAction* changeTheme;
    QAction* autoFormat;
    QLabel* fontTypeLabel;
    QFontComboBox* fontTypeCmb;
    QLabel* fontSizeLabel;
    QComboBox* fontSizeCmb;
    QToolButton* boldBtn;
    QToolButton* underlineBtn;

    QsciScintilla* textEdit;
    QsciLexer *textLexer;
    QTextCharFormat* tcf;
    Tree * tree1;
    QTimer* timi;


    QMenuBar *menuBar1;
    QToolBar *toolBar = addToolBar("Main Toolbar");


private:
    Ui::MainWindow *ui;
    QTableWidget *outputTable;

    //为真表示文件没有保存过，为假表示文件已经被保存过了
    bool isUnititled;
    //保存当前文件的路径
    QString curFile;
    //查找按钮点击后的窗口
    //Dialog对话框
    QDialog *findDlg;
    //单行输入框
    QLineEdit *findLineEdit;


    //minimap的四个对象
    QTextDocument textDocument;
    QPixmap codePixmap;
    QGraphicsPixmapItem *pixmapItem;
    QGraphicsScene *scene;
    QTableWidget *outputText;
    QGraphicsView *minimapView;
    QTabWidget *tabWidget;  //多文件窗体
    QTextEdit *te;
    QString fileName;       //文件路径
    int x;                  //横坐标
    int y;                  //纵坐标
    QLineEdit *changeEdit;
public slots:
    void op();
    void showMinimap();
//私有槽showFindText()声明
private slots:

    //创建窗口控件函数
    void createMenu();
    void createTool();
    void createTab();

    //所有的信号与槽连接函数
    void connectImpl();

    /******* 问题：为什么这个就不能写在public里面？？？ *******/

    //自定义槽函数：单机标签页
    void onTabClicked(int index);
    void onTabDoubleClicked(int index);
    void openThemeDialog();
    void doAutoFormat();

    //自定义槽函数-查找
    void showFindText();
    int KMPMatch(QString &text, QString &pattern);

    //自定义槽函数：设置主题
    void setTheme();
    void setScintilla(QsciScintilla *scintilla);

    //自定义槽函数-新建操作
    void newFile();
    //判断是否需要保存
    bool maybeSave();
    //自定义槽函数-保存操作
    bool save();
    //自定义槽函数-另存为操作
    bool saveasFile();
    //自定义槽函数-保存文件
    bool saveFile(const QString &fileName);
    //自定义槽函数-打开文件
    bool openFile(const QString &fileName);
    //自定义槽函数-字体加粗
    void setBold();
    //自定义槽函数-字体下划线
    void setUnderline();
    //自定义槽函数-编译文件
    int compile_file();
    void run_file();
    void compile_and_run();
    void FileNew();
    void undo();
    void redo();
    void changeText();
    void findNext();
    void changeThis();
    void findLast();
    //自定义槽函数-显示编译错误
    void displayCompileErr(cppCompileInfo* info);
    void focusOnPos(int row, int column);

};
#endif // MAINWINDOW_H

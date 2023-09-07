
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTableWidget>
#include <QMessageBox>
#include "cppcoderunner.h"
#include "cppcompileinfo.h"
#include "vector"
#include "stdlib.h"

int openTabNum = 0;
QString path;                                   // 定义一个全局变量存放地址
QTextCodec *codec;                              // 字符编码指针
QsciLexer *globalLexer;                         // 全局的格式
QsciScintilla *curScintilla;                    // 当前的火花
QMap<QString, QString>curTheme;                 // 全局主题设置
QMap<QString, QsciScintilla*> tabScintillaMap;  // tab与scintilla的对应
QMap<QString,QString> tabstringtopath;
QMap<QsciScintilla*, bool> isCompile;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 初始化文件为未保存状态
    isUnititled = true;
    // 初始化文件名为".c"
    curFile = tr("IDE");
    // 初始化窗口标题为文件名
    setWindowTitle(curFile);

    //设置窗口图标
    this->setWindowIcon(QIcon(":/icon/icon/editor.png"));
    //设置窗口大小
    this->resize(1200,600);

    tcf = new QTextCharFormat;

    codec = QTextCodec::codecForName("GBK");

    /********全局菜单栏建立*********/
    menuBar1 = this->menuBar();

    /************ 全局词法编辑器 ************/

    globalLexer = new QsciLexerCPP;
    //代码提示词全录入
    QsciAPIs *apis = new QsciAPIs(globalLexer);
    foreach (const QString &keyword, Keywords::keywords)
        apis->add(keyword);

    /************ 代码窗口建立 ************/
    tabWidget = new QTabWidget();
    QPushButton* addButton = new QPushButton("➕", tabWidget);
    addButton->setFixedSize(30, 30); // 设置按钮大小
    tabWidget->tabBar()->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    tabWidget->tabBar()->setFont(QFont("Consolas", 10)); // 将字体应用到 QTabWidget 的标签栏
    connect(addButton, &QPushButton::clicked, this, &MainWindow::createTab);
    tabWidget->addTab(new QWidget(), "");
    tabWidget->tabBar()->setTabButton(0, QTabBar::LeftSide, addButton);

    /************ 文件数与小地图建立 ************/
    tree1 = new Tree;
      minimapView = new QGraphicsView;
    minimapView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    minimapView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    minimapView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    createMenu();
    createTool();
    createTab();

    /************ 布局绑定 ************/

    // 创建一个垂直布局管理器
    QVBoxLayout *mainLayout = new QVBoxLayout;
    // 创建一个水平布局管理器，用于容纳文件树和右边所有
    QHBoxLayout *horizontalLayout = new QHBoxLayout;

    // 创建一个小QSplitter，用于容纳地图代码框和代码文件框
    QSplitter *smallsplitter = new QSplitter(Qt::Horizontal);;
    smallsplitter->addWidget(tabWidget);
    smallsplitter->addWidget(minimapView);
    smallsplitter->setStretchFactor(0, 7); // 第一个部分（textEdit）的伸缩因子为7
    smallsplitter->setStretchFactor(1, 1); // 第二个部分（graphicText）的伸缩因子为1

    // 将树形部件添加到水平布局中
    horizontalLayout->addWidget(tree1,1);

    // 创建一个QSplitter来容纳地图和代码编辑器的整体和输出结果文本框
    QSplitter *splitter = new QSplitter(Qt::Vertical);

    // 添加smallQSplitter到QSplitter
    splitter->addWidget(smallsplitter);

    // 创建输出结果表格
    outputTable = new QTableWidget(0, 5, this);
    outputTable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // 添加输出结果表格到QSplitter
    splitter->addWidget(outputTable);
    //设置代码页与编译窗口比例
    QList<int> sizes;
    sizes << 25000 << 10000;
    splitter->setSizes(sizes);

    //设置表头
    outputTable->setHorizontalHeaderItem(0, new QTableWidgetItem("文件"));
    outputTable->setHorizontalHeaderItem(1, new QTableWidgetItem("行号"));
    outputTable->setHorizontalHeaderItem(2, new QTableWidgetItem("列号"));
    outputTable->setHorizontalHeaderItem(3, new QTableWidgetItem("错误类型"));
    outputTable->setHorizontalHeaderItem(4, new QTableWidgetItem("详细信息"));

    // 添加输出结果文本框到QSplitter
    splitter->addWidget(outputTable);
    splitter->setStretchFactor(0, 11); // 第一个部分（textEdit）的伸缩因子为7
    splitter->setStretchFactor(1, 1); // 第二个部分（graphicText）的伸缩因子为1
    // 将QSplitter添加到水平布局中
    horizontalLayout->addWidget(splitter,4);
    // 创建一个占位的小部件到垂直布局中，以确保输出结果文本框位于底部
    mainLayout->addLayout(horizontalLayout);
    // 创建一个中央部件（QWidget）来容纳布局
    QWidget *centralWidget = new QWidget;
    centralWidget->setContentsMargins(0, 0, 0, 0);
    centralWidget->setLayout(mainLayout);
    // 将中央部件设置为主窗口的中央部件
    setCentralWidget(centralWidget);

    //设置状态栏
    QStatusBar* statusBar = this->statusBar();

    QLabel* authorLabel = new QLabel("作者：不会取名小组");
    authorLabel->setAlignment(Qt::AlignLeft);
    statusBar->addPermanentWidget(authorLabel);

    //点击查找后显示的窗口
    findDlg = new QDialog(this);
    findDlg->setWindowTitle(tr("查找"));
    findLineEdit = new QLineEdit(findDlg);
    changeEdit = new QLineEdit(findDlg);
    QPushButton *btn1= new QPushButton(tr("查找第一个"), findDlg);
    QPushButton *btn3= new QPushButton(tr("查找下一个"), findDlg);
    QPushButton *btn5= new QPushButton(tr("查找上一个"), findDlg);
    QPushButton *btn4= new QPushButton(tr("替换此处"), findDlg);
    QPushButton *btn2= new QPushButton(tr("替换全部"), findDlg);
    //垂直布局
    QVBoxLayout *layout1= new QVBoxLayout(findDlg);
    layout1->addWidget(findLineEdit);
    layout1->addWidget(changeEdit);
    layout1->addWidget(btn1);
    layout1->addWidget(btn3);
    layout1->addWidget(btn5);
    layout1->addWidget(btn4);
    layout1->addWidget(btn2);
    //将“查找下一个”按钮与自定义showFindText槽函数连接
    connect(btn1, SIGNAL(clicked()), this, SLOT(showFindText()));
    connect(btn3, SIGNAL(clicked()), this, SLOT(findNext()));
    connect(btn2, SIGNAL(clicked()), this, SLOT(changeText()));
    connect(btn4, SIGNAL(clicked()), this, SLOT(changeThis()));
    connect(btn5, SIGNAL(clicked()), this, SLOT(findLast()));
    connectImpl();

    /************ 迷你地图 ************/

    // 创建一个 QTextDocument
    textDocument.setPlainText(QString()); // 初始文本为空

    // 创建一个 QPixmap
    int scaleFactor = 1; // 增加分辨率的倍数
    codePixmap = QPixmap(textDocument.size().toSize() * scaleFactor);
    codePixmap.fill(Qt::white); // 填充白色背景

    // 创建一个 QGraphicsPixmapItem
    pixmapItem = new QGraphicsPixmapItem(codePixmap);

    // 创建一个 QGraphicsScene
    scene = new QGraphicsScene();
    scene->addItem(pixmapItem);

    // 设置 QGraphicsView 显示的区域和图像
    minimapView->setScene(scene);
    // 设置
    //this->setStyleSheet("background-color: #FF0000;");
    curTheme = Theme::Default;
    setTheme();

}
void MainWindow::createTab() {

    // 创建新的对象
    QsciScintilla* newScintilla = new QsciScintilla; // 创建一个新的 QsciScintilla 对象
    newScintilla->setLexer(globalLexer); // 设置词法分析器（可能是您在其他地方定义的）
    QString tabTitle = "Tab" + QString::number(openTabNum);
    tabstringtopath[tabTitle] = "C:\\Document:\\yuanshen\\qidong"; //这里面写电脑里面有的东西
    openTabNum ++;
    tabWidget->addTab(newScintilla, tabTitle);
    tabScintillaMap.insert(tabTitle, newScintilla);
    qDebug()<<tabTitle<<newScintilla;

    // 添加关闭按钮到新标签页
    QWidget* tabWidgetContainer = new QWidget();
    QHBoxLayout* tabLayout = new QHBoxLayout(tabWidgetContainer);
    QPushButton* closeButton = new QPushButton("✖"); // 使用✖作为关闭按钮的文本，可以替换为图标

    // 设置关闭按钮的固定大小（例如：25x25 像素）
    closeButton->setFixedSize(25, 25);

    tabLayout->addWidget(closeButton);
    tabLayout->setContentsMargins(0, 0, 0, 0);
    tabLayout->setSpacing(0);

    tabWidget->setTabText(tabWidget->count() - 1, tabTitle);
    tabWidget->tabBar()->setTabButton(tabWidget->count() - 1, QTabBar::RightSide, tabWidgetContainer);

    // 连接按钮的点击事件到关闭标签页的槽函数
    connect(closeButton, &QPushButton::clicked, [this, newScintilla]() {
        // 获取当前标签页的索引
        int currentIndex = tabWidget->indexOf(newScintilla);

        // 关闭标签页
        tabWidget->removeTab(currentIndex);

        // 删除相应的 QsciScintilla 对象
        //delete newScintilla;
    });

    // 切换到最新的这个 tab
    tabWidget->setCurrentIndex(tabWidget->count() - 1);

    setScintilla(newScintilla);

    curScintilla = newScintilla;

}
void MainWindow::createMenu()
{
    //基本控件设置
    //获取菜单栏
    QMenuBar *menuBar = this->menuBar();

    //定义文件菜单和编辑菜单
    QMenu* fileMenu = menuBar->addMenu("文件");
    QMenu* EditMenu =menuBar->addMenu("编辑");
    QMenu* CompileMenu =menuBar->addMenu("编译");
//    QMenu* filenew =menuBar->addMenu("新建窗口");
//    QAction *file_new = new QAction(QIcon(":/."),"新建窗口");
//    filenew->addAction(file_new);

    te = new QTextEdit;
    this->setCentralWidget(te);
    this->setMinimumSize(400,300);      //最小大小
    this->resize(800,600);              //默认大小
//    connect(file_new,SIGNAL(triggered(bool)),this,SLOT(FileNew()));


    //向文件菜单中添加行为
    newfile = fileMenu->addAction(QIcon(":/icon/icon/newfile.png"),"新建");
    openfile = fileMenu->addAction(QIcon(":/icon/icon/openfile.png"),"打开");
    savefile = fileMenu->addAction(QIcon(":/icon/icon/save.png"),"保存");
    saveasfile = fileMenu->addAction(QIcon(":/icon/icon/newsave.png"),"另存为");

    //向编辑菜单中添加行为
    copyText = EditMenu->addAction(QIcon(":/icon/icon/copy.png"),"复制");
    cutText = EditMenu->addAction(QIcon(":/icon/icon/cut.png"),"剪切");
    pasteText = EditMenu->addAction(QIcon(":/icon/icon/paste.png"),"粘贴");
    EditMenu->addSeparator();
    seekText = EditMenu->addAction(QIcon(":/icon/icon/seek.png"),"查找文本");
    EditMenu->addSeparator();
    fontSet = EditMenu->addAction(QIcon(":/icon/icon/fontSet.png"),"字体设置");
    undoe = EditMenu->addAction(QIcon(":/icon/icon/undo.png"),"撤销");
    redoe = EditMenu->addAction(QIcon(":/icon/icon/redo.png"),"重做");
    changeTheme = EditMenu->addAction(QIcon(":/icon/icon/change color.png"),"主题");
    autoFormat = EditMenu->addAction(QIcon(":/icon/icon/format.png"),"自动排版");

    //向编译菜单中添加行为
    compilefile = CompileMenu->addAction(QIcon(":/icon/icon/compile.png"),"编译");
    runfile = CompileMenu->addAction(QIcon(":/icon/icon/run.png"),"运行");
    compileAndRun = CompileMenu->addAction(QIcon(":/icon/icon/compile_and_run.png"),"编译并运行");



}
void MainWindow::createTool()
{
    //添加工具栏
    QToolBar* toolBar = this->addToolBar("tool");

    //工具栏添加新建、打开、保存、另存为
    toolBar->addAction(newfile);
    toolBar->addSeparator();
    toolBar->addAction(openfile);
    toolBar->addSeparator();
    toolBar->addAction(savefile);
    toolBar->addSeparator();
    toolBar->addAction(saveasfile);
    toolBar->addSeparator();

    //工具栏添加复制、剪切、粘贴、查找、字体设置
    toolBar->addAction(copyText);
    toolBar->addSeparator();
    toolBar->addAction(cutText);
    toolBar->addSeparator();
    toolBar->addAction(pasteText);
    toolBar->addSeparator();
    toolBar->addAction(seekText);
    toolBar->addSeparator();
    toolBar->addAction(changeTheme);
    toolBar->addSeparator();
    toolBar->addAction(autoFormat);
    toolBar->addSeparator();
    toolBar->addAction(compilefile);
    toolBar->addSeparator();
    toolBar->addAction(runfile);
    toolBar->addSeparator();
    toolBar->addAction(compileAndRun);
    toolBar->addSeparator();


    /************ 工具栏的字体设置 ************/
/*
    //设置“字体”标签
    fontTypeLabel = new QLabel("字体：");
    toolBar->addWidget(fontTypeLabel);
    //添加字体框
    fontTypeCmb = new QFontComboBox;
    toolBar->addWidget(fontTypeCmb);

    toolBar->addSeparator();

    //设置“字号”标签
    fontSizeLabel = new QLabel("字号：");
    toolBar->addWidget(fontSizeLabel);
    //添加字号框
    fontSizeCmb = new QComboBox;
    toolBar->addWidget(fontSizeCmb);
    //字号框中添加字号选项
    for(int i=1;i<=72;i++){
        fontSizeCmb->addItem(QString::number(i));
    }

    toolBar->addSeparator();

    //添加加粗按钮
    boldBtn = new QToolButton();
    boldBtn->setToolTip("加粗");
    boldBtn->setIcon(QIcon(":/icon/icon/bold.png"));
    toolBar->addWidget(boldBtn);

    toolBar->addSeparator();

    //添加下划线按钮
    underlineBtn = new QToolButton();
    underlineBtn->setToolTip("下划线");
    underlineBtn->setIcon(QIcon(":/icon/icon/underline.png"));
    toolBar->addWidget(underlineBtn);

    */
}

/************* 信号与槽的连接 *************/

void MainWindow::connectImpl()
{
    //单击某个tab的槽函数
    connect(tabWidget->tabBar(), SIGNAL(tabBarClicked(int)), this, SLOT(onTabClicked(int)));

    //双击某个tab的槽函数
    connect(tabWidget->tabBar(), SIGNAL(tabBarDoubleClicked(int)), this, SLOT(onTabDoubleClicked(int)));

    //修改主题的槽函数
    connect(changeTheme, &QAction::triggered, this, &MainWindow::openThemeDialog);

    //修改主题的槽函数
    connect(autoFormat, &QAction::triggered, this, &MainWindow::doAutoFormat);



    //信号与槽-新建文件
    connect(newfile,QAction::triggered,this,MainWindow::newFile);


    //信号与槽-打开文件 //C++11特性：lambda表达式 匿名函数
    connect(openfile,QAction::triggered,[=]{
        if(maybeSave()){
            QString fileName = QFileDialog::getOpenFileName(this);

            if(!fileName.isEmpty()){
                openFile(fileName);
            }
        }
    });

    //信号与槽-保存文件
    connect(savefile,QAction::triggered,this,MainWindow::save);

    //信号与槽-文件另存为
    connect(saveasfile,QAction::triggered,this,MainWindow::saveasFile);

    // 信号与槽-复制
    //    connect(copyText, &QAction::triggered, textEdit, &QsciScintilla::copy);

    // 连接按钮的点击信号到槽函数
    connect(copyText, &QAction::triggered, this, [=]() {
        // 执行QScintilla的复制操作
        curScintilla->copy();
    });

    // 信号与槽-剪切
    //    connect(cutText, &QAction::triggered, textEditor, &QsciScintilla::cut);

    connect(cutText, &QAction::triggered, this, [=]() {
        // 执行QScintilla的复制操作
        curScintilla->cut();
    });

    // 信号与槽-粘贴
    //    connect(pasteText, &QAction::triggered, textEditor, &QsciScintilla::paste);

    connect(pasteText, &QAction::triggered, this, [=]() {
        // 执行QScintilla的复制操作
        curScintilla->paste();
    });

    //信号与槽-搜索
    connect(seekText,QAction::triggered,[=]{
        findDlg->show();
    });
    //信号与槽-字体设置
    connect(fontSet,QAction::triggered,[=]{
        bool fontSelected;
        QFont font = QFontDialog::getFont(&fontSelected,this);
        if(fontSelected){
            curScintilla->setFont(font);
        }
    });

    /************ 工具栏的字体设置 ************/
/*
    //信号与槽-字体改变
    connect(fontTypeCmb,QFontComboBox::currentFontChanged,this,MainWindow::setFont);

    //信号与槽-字号改变
    void (QComboBox::*p)(int)=QComboBox::currentIndexChanged;
    connect(fontSizeCmb,p,this,MainWindow::setFontSize);

    //信号与槽-字体加粗
    connect(boldBtn,QToolButton::clicked,this,MainWindow::setBold);

    //信号与槽-字体下划线
    connect(underlineBtn,QToolButton::clicked,this,MainWindow::setUnderline);
    //    (QTreeWidgetItem *item, int column)

*/


    //信号与槽-文件编译
    connect(compilefile,QAction::triggered,this,MainWindow::compile_file);
    connect(runfile,QAction::triggered,this,MainWindow::run_file);
    connect(compileAndRun,QAction::triggered,this,MainWindow::compile_and_run);
    connect(undoe,QAction::triggered,this,MainWindow::undo);
    connect(redoe,QAction::triggered,this,MainWindow::redo);
    //点击编译信息跳转
    connect(outputTable, &QTableWidget::cellDoubleClicked, this, &MainWindow::focusOnPos);
}

/*************  自定义槽函数的实现 *************/

void MainWindow::doAutoFormat(){
    QProcess process;
    process.setProgram("cmd");
    QStringList arguments;
    qDebug()<<curFile;
    arguments << "/c" << "clang-format" << "-style={BasedOnStyle: llvm, IndentWidth: 4}" << "-i" << curFile;
    qDebug()<<arguments;
    process.setArguments(arguments);
    process.start();
    process.waitForFinished();
    QFile f(curFile);
    if(f.open(QIODevice::ReadWrite)){
        QTextStream in(&f);
        QString fcontent = in.readAll();
        curScintilla->setText(fcontent);
        f.close();
    }else{
        qDebug()<<"reload file failed!";
        return;
    }
    return;

}
// 换主题
void MainWindow::setTheme(){

    qDebug()<<"ok0";
    this->setStyleSheet("background-color: "+curTheme["o_mainwindow_bgc"]+";");

    //修改菜单栏和工具栏背景
    QString style2 = QString("background-color: %1;color: %2").arg(curTheme["o_tree_bgc"]).arg(curTheme["o_tree_fontc"]);
    menuBar1->setStyleSheet(style2);
    toolBar->setStyleSheet(style2);

    //先换其他三块的主题配色

    minimapView->setStyleSheet("background-color: "+curTheme["o_minimap_bgc"]+"; color:"+curTheme["o_minimap_fontc"]+";");
    tree1->setStyleSheet("background-color: "+curTheme["o_tree_bgc"]+"; color:"+curTheme["o_tree_fontc"]+";");
    // 设置 QTableWidget 的背景色和字体颜色
    outputTable->setStyleSheet("background-color: " + curTheme["o_output_bgc"] + "; color: " + curTheme["o_output_fontc"] + ";");

    // 设置表头背景色和字体颜色
    QString headerStyleSheet = "QHeaderView::section {"
                               "    background-color: " + curTheme["o_output_bgc"] + ";"
                               "    color: " + curTheme["o_output_fontc"] + ";"
                               "}";
    outputTable->horizontalHeader()->setStyleSheet(headerStyleSheet);

    qDebug()<<"ok1";

    //再换词法编辑器lexer
    globalLexer->setColor(curTheme["l_color"]);                                    //前景色
    globalLexer->setPaper(curTheme["l_paper"]);                                    //背景色
    globalLexer->setColor(QColor(curTheme["l_comment"]),QsciLexerCPP::Comment);
    globalLexer->setColor(QColor(curTheme["l_comment"]), QsciLexerCPP::CommentLine);
    globalLexer->setColor(QColor(curTheme["l_comment"]), QsciLexerCPP::CommentLineDoc);
    globalLexer->setColor(QColor(curTheme["l_quote"]), QsciLexerCPP::DoubleQuotedString);        //设置双引号内容
    globalLexer->setColor(QColor(curTheme["l_quote"]), QsciLexerCPP::SingleQuotedString);        //设置双引号内容
    globalLexer->setColor(QColor(curTheme["l_keyword"]), QsciLexerCPP::Keyword);                       //设置关键词
    globalLexer->setColor(QColor(curTheme["l_number"]), QsciLexerCPP::Number);                          //设置数字
    globalLexer->setColor(QColor(curTheme["l_preprocessor"]),QsciLexerCPP::PreProcessor);               //预处理
    globalLexer->setColor(QColor(curTheme["l_operator"]),QsciLexerCPP::Operator);
    qDebug()<<"ok2";


    //最后遍历所有的scintilla对象
    foreach (const QString& tabName, tabScintillaMap.keys()) {
        QsciScintilla* scintilla = tabScintillaMap.value(tabName); // 获取QsciScintilla对象
        scintilla->setLexer(globalLexer);
        // 设置选中文本的字体与背景
        scintilla->setSelectionForegroundColor(curTheme["s_select_fontc"]);
        scintilla->setSelectionBackgroundColor(curTheme["s_select_bgc"]);
        // 行号栏的字体与背景
        scintilla->setMarginsForegroundColor(curTheme["s_margin_fontc"]);
        scintilla->setMarginsBackgroundColor(curTheme["s_margin_bgc"]);
        // 设置匹配的括号的字体与背景
        scintilla->setMatchedBraceForegroundColor(curTheme["s_match_fontc"]);
        scintilla->setMatchedBraceBackgroundColor(curTheme["s_match_bgc"]);
        // 设置未匹配的括号的字体与背景
        scintilla->setUnmatchedBraceForegroundColor(curTheme["s_unmatch_fontc"]);
        scintilla->setUnmatchedBraceBackgroundColor(curTheme["s_unmatch_bgc"]);
        // 设置折叠区域的背景色
        scintilla->setFoldMarginColors(curTheme["s_folding_bgc"], curTheme["s_folding_bgc"]);
    }
    qDebug()<<"ok3";

    globalLexer->setPaper(curTheme["l_paper"]);
}

void MainWindow::setScintilla(QsciScintilla *scintilla){

    scintilla->setLexer(globalLexer);
    // 设置自动补全，自动补全所有地方出现的
    scintilla->setAutoCompletionSource(QsciScintilla::AcsAPIs);
    scintilla->setAutoCompletionThreshold(2);
    // 设置严格的括号匹配
    scintilla->setBraceMatching(QsciScintilla::StrictBraceMatch);
    // 设置为UTF-8编码
    scintilla->setUtf8(true);
    // 设置自动补全大小写敏感
    scintilla->setAutoCompletionCaseSensitivity(true);
    // 设置自动缩进
    scintilla->setAutoIndent(true);
    // 启用显示缩进引导线
    scintilla->setIndentationGuides(true);
    // 设置当前行的光标宽度和颜色
    scintilla->setCaretWidth(2); // 光标宽度，0表示不显示光标
    scintilla->setCaretForegroundColor(QColor("darkCyan")); // 光标颜色
    scintilla->setCaretLineVisible(true); // 是否高亮显示光标所在行
    // 设置左侧行号栏的宽度等
    QFont font("Consolas", 10, QFont::Normal);
    QFontMetrics fontmetrics = QFontMetrics(font);
    scintilla->setMarginWidth(0, fontmetrics.width("00000"));
    scintilla->setMarginLineNumbers(0, true);
    // 启用匹配括号的高亮显示
    scintilla->setBraceMatching(QsciScintilla::StrictBraceMatch);
    scintilla->setTabWidth(4);
    QFont margin_font;
    margin_font.setFamily("Consolas");
    margin_font.setPointSize(11);
    scintilla->setMarginsFont(QFont("Consolas")); // 设置页边字体
    scintilla->setMarginType(0, QsciScintilla::NumberMargin); // 设置标号为0的页边显示行号
    // 设置选中文本的字体与背景
    scintilla->setSelectionForegroundColor(curTheme["s_select_fontc"]);
    scintilla->setSelectionBackgroundColor(curTheme["s_select_bgc"]);
    // 行号栏的字体与背景
    scintilla->setMarginsForegroundColor(curTheme["s_margin_fontc"]);
    scintilla->setMarginsBackgroundColor(curTheme["s_margin_bgc"]);
    // 设置匹配的括号的字体与背景
    scintilla->setMatchedBraceForegroundColor(curTheme["s_match_fontc"]);
    scintilla->setMatchedBraceBackgroundColor(curTheme["s_match_bgc"]);
    // 设置未匹配的括号的字体与背景
    scintilla->setUnmatchedBraceForegroundColor(curTheme["s_unmatch_fontc"]);
    scintilla->setUnmatchedBraceBackgroundColor(curTheme["s_unmatch_bgc"]);
    // 设置折叠区域的背景色
    scintilla->setFoldMarginColors(curTheme["s_folding_bgc"], curTheme["s_folding_bgc"]);

    globalLexer->setPaper(curTheme["l_paper"]);
}

void MainWindow::openThemeDialog()
{
    // 创建主题选择对话框
    QDialog themeDialog(this);
    themeDialog.setWindowTitle("主题选择");

    // 创建九宫格布局
    QGridLayout *gridLayout = new QGridLayout(&themeDialog);

    // 主题名称列表
    QStringList themes = {
        "米", "白", "蓝",
        "绿", "紫", "粉",
        "深", "马卡龙", "莫兰迪"
    };

    // 创建按钮并添加到九宫格布局中
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            int themeIndex = i * 3 + j; // 主题索引
            QPushButton *themeButton = new QPushButton(themes[themeIndex]);

            // 设置按钮为圆形
            int buttonSize = 120; // 按钮的大小，根据需要调整
            themeButton->setFixedSize(buttonSize, buttonSize);

            // 设置按钮的字体为微软雅黑
            themeButton->setFont(QFont("Microsoft YaHei", 12));

            // 设置不同的背景颜色和悬停/按下效果
            QString backgroundColor;
            QString hoverColor;
            QString pressedColor;

            switch (themeIndex) {
                case 0: // 米色
                    backgroundColor = "#F5DEB3";
                    hoverColor = "#D2B48C";
                    pressedColor = "#B8860B";
                    break;
                case 1: // 白色
                    backgroundColor = "#FFFFFF";
                    hoverColor = "#E0E0E0";
                    pressedColor = "#C0C0C0";
                    break;
                case 2: // 蓝色
                    backgroundColor = "#ADD8E6";
                    hoverColor = "#87CEEB";
                    pressedColor = "#6495ED";
                    break;
                case 3: // 绿色
                    backgroundColor = "#90EE90";
                    hoverColor = "#98FB98";
                    pressedColor = "#00FF7F";
                    break;
                case 4: // 紫色
                    backgroundColor = "#D8BFD8";
                    hoverColor = "#C0A9D8";
                    pressedColor = "#836FFF";
                    break;
                case 5: // 粉色
                    backgroundColor = "#FFB6C1";
                    hoverColor = "#FFC0CB";
                    pressedColor = "#FF69B4";
                    break;
                case 6: // 灰色
                    backgroundColor = "#808080";
                    hoverColor = "#A9A9A9";
                    pressedColor = "#696969";
                    break;
                case 7: // 马卡龙色
                    backgroundColor = "#FFD700";
                    hoverColor = "#FFC125";
                    pressedColor = "#FFD700";
                    break;
                case 8: // 莫兰迪色
                    backgroundColor = "#EEE8AA";
                    hoverColor = "#F0E68C";
                    pressedColor = "#DAA520";
                    break;
                default:
                    break;
            }

            // 设置按钮样式
            QString buttonStyle = "QPushButton {"
                                  "    border-radius: " + QString::number(buttonSize / 2) + "px;"
                                  "    background-color: " + backgroundColor + ";"
                                  "}"
                                  "QPushButton:hover {"
                                  "    background-color: " + hoverColor + ";"
                                  "}"
                                  "QPushButton:pressed {"
                                  "    background-color: " + pressedColor + ";"
                                  "}";
            themeButton->setStyleSheet(buttonStyle);

            connect(themeButton, &QPushButton::clicked, [themeIndex, this]() {
                switch (themeIndex) {
                case 0:
                    curTheme = Theme::Default;
                    setTheme();
                    break;
                case 1:
                    curTheme = Theme::White;
                    setTheme();
                    break;
                case 2:
                    curTheme = Theme::Blue;
                    setTheme();
                    break;
                case 3:
                    curTheme = Theme::Green;
                    setTheme();
                    break;
                case 4 :
                    curTheme = Theme::Purple;
                    setTheme();
                    break;
                case 5:
                    curTheme = Theme::Pink;
                    setTheme();
                    break;
                case 6:
                    curTheme = Theme::Dark;
                    setTheme();
                    break;
                case 7:
                    curTheme = Theme::Macaron;
                    setTheme();
                    break;
                case 8:
                    curTheme = Theme::Morandi;
                    setTheme();
                    break;

                }

                        });
            gridLayout->addWidget(themeButton, i, j);
        }
    }

    // 显示对话框
    themeDialog.exec();
}

int MainWindow::KMPMatch( QString &text, QString &pattern) {
    int m = text.length();
    int n = pattern.length();

    if (n == 0) {
        return 0; // 如果查找文本为空，返回0位置
    }

    QVector<int> lps(n); // 部分匹配表

    // 构建部分匹配表
    int len = 0; // 当前匹配的字符数
    lps[0] = 0; // 第一个字符的部分匹配值总是0

    for (int i = 1; i < n; i++) {
        while (len > 0 && pattern[i] != pattern[len]) {
            len = lps[len - 1];
        }

        if (pattern[i] == pattern[len]) {
            len++;
        }

        lps[i] = len;
    }

    // 开始匹配
    int i = 0; // 指向text的指针
    int j = 0; // 指向pattern的指针

    while (i < m) {
        if (pattern[j] == text[i]) {
            i++;
            j++;
        }

        if (j == n) {
            return i - j; // 找到匹配项，返回匹配的位置
        } else if (i < m && pattern[j] != text[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }

    return -1; // 未找到匹配项
}

void MainWindow::showMinimap() {
    // 获取 QScintilla 控件的内容
    QString codeText = curScintilla->text();
    QFont font = curScintilla->font(); // 获取 QScintilla 的字体
    int lineHeight = QFontMetrics(font).lineSpacing(); // 获取字体的行高度
    int windowHeight = tabWidget->height();
    int firstLineNumber = curScintilla->firstVisibleLine();
    int lastLineNumber = firstLineNumber + windowHeight / lineHeight;

    // 更新 QTextDocument 的内容
    textDocument.setPlainText(codeText);

    // 获取代码文本的大小
    QSizeF textSize = textDocument.size();

    // 更新 QPixmap 的大小
    int scaleFactor = 1; // 增加分辨率的倍数
    codePixmap = QPixmap(textSize.toSize() * scaleFactor);
    codePixmap.fill(Qt::white); // 填充白色背景
    QPainter painter(&codePixmap);
    painter.setRenderHint(QPainter::TextAntialiasing); // 启用文本抗锯齿
    painter.scale(scaleFactor, scaleFactor); // 缩放以适应分辨率

    // 在副本图像上方绘制红色长方形
    painter.setPen(Qt::NoPen); // 取消画笔，以绘制填充
    painter.setBrush(QColor(0, 255, 0, 100)); // 设置红色填充颜色，透明度为 100
    int boxX = 0;
    int boxY = firstLineNumber * lineHeight;
    int boxWidth = codePixmap.width(); // 红色方框宽度等于代码图像的宽度
    int boxHeight = (lastLineNumber - firstLineNumber + 1) * lineHeight;
    painter.drawRect(boxX, boxY, boxWidth, boxHeight);

    // 创建一个粗体 QFont
    QFont boldFont = curScintilla->font();
    boldFont.setBold(true);

    // 将粗体 QFont 应用到 QPainter
    painter.setFont(boldFont);

    // 设置笔的宽度为最大值
    QPen pen = painter.pen();
    pen.setWidth(5); // 设置笔的宽度为 10，用于绘制代码文本
    painter.setPen(pen);

    textDocument.drawContents(&painter);

    // 更新 QGraphicsPixmapItem 的图像
    pixmapItem->setPixmap(codePixmap);

    // 缩放图像以适应 MinimapView 的宽度
    double viewWidth = minimapView->viewport()->width();
    double scaleFactorX = viewWidth / (textSize.width() * scaleFactor);
    double scaleFactorY = viewWidth / (textSize.width() * scaleFactor);
    minimapView->resetMatrix();
    minimapView->scale(scaleFactorX, scaleFactorY);

    // 设置图像的对齐方式，使其垂直对齐到顶部
    minimapView->setAlignment(Qt::AlignTop);

    // 如果图像高度大于 MinimapView 的高度，显示垂直滚动条

    /*
    if (textSize.height() * scaleFactorY > minimapView->viewport()->height()) {
        minimapView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    } else {
        minimapView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }*/
}

// 单击Tab：切换到当前这个tab
void MainWindow::onTabClicked(int index) {
    QString tabTitle = tabWidget->tabText(index);
    qDebug()<<tabTitle;
    QsciScintilla *tabScintilla = tabScintillaMap[tabTitle];
    qDebug()<<tabScintilla;
    curScintilla = tabScintilla;
    qDebug()<<"成功切换";
    QString curFile = QFileInfo(tabstringtopath[tabTitle]).canonicalFilePath();
    QDir currentDir(curFile);
//    qDebug()<<tabnumtopath[index];
    currentDir.cdUp(); // 切换到上级目录
    qDebug()<< curFile;
    QString parentDir = currentDir.canonicalPath(); // 获取上级目录的规范路径
    int number = tree1->root->childCount();
    for(int i = number - 1; i >= 0; i--){
        tree1->root->removeChild(tree1->root->child(i));
    }
    tree1->root->setText(0,parentDir);
    qDebug()<<parentDir;
    QFileInfoList filelist = tree1->allfile(tree1->root,parentDir);
    /*
    QString tabTitle = tabWidget->tabText(index);
    qDebug()<<tabTitle;
    QsciScintilla *tabScintilla = tabScintillaMap[tabTitle];
    qDebug()<<tabScintilla;
    curScintilla = tabScintilla;
    showMinimap();
    qDebug()<<"成功切换";
    QString curFile = QFileInfo(fileName).canonicalFilePath();
    QDir currentDir(curFile);
    currentDir.cdUp(); // 切换到上级目录
    QString parentDir = currentDir.canonicalPath(); // 获取上级目录的规范路径
    int number = tree1->root->childCount();
    for(int i = number - 1; i >= 0; i--){
        tree1->root->removeChild(tree1->root->child(i));
    }
    tree1->root->setText(0,parentDir);
    qDebug()<<parentDir;
    QFileInfoList filelist = tree1->allfile(tree1->root,parentDir);*/
}

// 双击tab，修改标签title
void MainWindow::onTabDoubleClicked(int index) {
    QString tabTitle = tabWidget->tabText(index);
    QsciScintilla *tabScintilla = tabScintillaMap[tabTitle];
    curScintilla = tabScintilla;
    qDebug() << "成功切换";

    bool ok;
    QString newTabTitle = QInputDialog::getText(this, "修改标签", "请输入新标签", QLineEdit::Normal, tabTitle, &ok);

    if (ok && !newTabTitle.isEmpty() && tabScintillaMap.find(newTabTitle)== tabScintillaMap.end()) {
        tabScintillaMap.remove(tabTitle);
        tabScintillaMap.insert(newTabTitle,tabScintilla);
        tabWidget->setTabText(index, newTabTitle);
    }
    else if(ok){
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("该名称重复或不可用，请重新输入");
        msgBox.setWindowTitle("错误");
        msgBox.exec();
    }

}

// 看样子是打开函数的方法
void MainWindow::op()
{
    //    qDebug() << "yes";
    if(tree1->filePath != "null"){
        QFileInfo fileInfo(tree1->filePath);
        if(fileInfo.isDir()){
            tree1->filePath = "null";
            return;
        }
        createTab();
        int index = tabWidget->currentIndex();
        //袁泽龙的修改
        QString tabTitle = tabWidget->tabText(index);
        QsciScintilla *tabScintilla = tabScintillaMap[tabTitle];
        int lastIndex = tree1->filePath.lastIndexOf("/");
        QString newTabTitle;
        if (lastIndex != -1) {
            // 使用mid函数截取最后一个斜杠后面的文件名
            newTabTitle = tree1->filePath.mid(lastIndex + 1);
        } else {
            // 未找到斜杠，说明地址字符串不包含路径信息
            newTabTitle = tree1->filePath;
        }
        tabScintillaMap.remove(tabTitle);
        tabScintillaMap.insert(newTabTitle, tabScintilla);
        tabWidget->setTabText(index, newTabTitle);
        tabstringtopath[newTabTitle] = tree1->filePath;

        //修改结束
        qDebug()<<"open  "<<tree1->filePath;
        curFile = tree1->filePath;
//        fileName = tree1->filePath;
        QFile f(tree1->filePath);
        if(f.open(QIODevice::ReadWrite)){
            QTextStream in(&f);
            QString fcontent = in.readAll();
            curScintilla->setText(fcontent);
            f.close();
        }else{
            qDebug()<<"open file failed!";
            //               return;
        }
        tree1->filePath = "null";
        return;
    }
    qDebug();
}

void MainWindow::undo()
{
    qDebug()<<curScintilla;
    curScintilla->undo();
}

void MainWindow::redo()
{
    curScintilla->redo();
}

//新建窗口
void MainWindow::FileNew()
{
    qDebug()<<"新窗口";
    x = 0;
    y = 0;
    x = this->geometry().x() + 25;
    y = this->geometry().y() + 25;
    MainWindow *bbq;               //新窗口
    bbq = new MainWindow;
    bbq->setWindowTitle("IDE");
    bbq->move(x,y);
    bbq->show();
}

//槽函数实现-新建文件
void MainWindow::newFile(){
    isUnititled = true;
    curFile = tr(".c");
    setWindowTitle(curFile);
    createTab();
}

//判断是否保存过
bool MainWindow::maybeSave(){
    //如果文档被更改了
    if(curScintilla->isModified()){
        //自定义一个警告对话框
        QMessageBox box;
        box.setWindowTitle(tr("警告"));
        box.setIcon(QMessageBox::Warning);
        box.setText(curFile+tr(" 尚未保存，是否保存？"));
        QPushButton *yesBtn = box.addButton(tr("是(&Y)"),QMessageBox::YesRole);
        box.addButton(tr("否(&N)"),QMessageBox::NoRole);
        QPushButton *cancelBtn = box.addButton(tr("取消"),QMessageBox::RejectRole);
        box.exec();
        if(box.clickedButton()==yesBtn)
            return save();
        else if(box.clickedButton()==cancelBtn)
            return false;
    }
    //如果文档没被修改，则返回true
    return true;
}

//Todo: bug写入数据保存文件之后再点击保存按钮会提示未保存！！！！！！
//如果文档以前没有保存过，那么执行另存为操作saveasFile()
//如果已经保存过，那么调用saveFile()执行文件保存操作。
bool MainWindow::save(){
    if(isUnititled){
        return saveasFile();
    }else {
        return saveFile(curFile);
    }
}

//槽函数实现-另存为操作，传入文件名调用真正的保存文件操作
bool MainWindow::saveasFile(){
    QString fileName = QFileDialog::getSaveFileName(this,tr("另存为"),curFile);
    if(fileName.isEmpty())
        return false;
    path=fileName;
    return saveFile(fileName);
}

//槽函数实现-真正的保存文件操作
bool MainWindow::saveFile(const QString &fileName){
    QFile f(fileName);

    if(f.open(QIODevice::ReadWrite)){
        QTextStream fin(&f);
        fin<<curScintilla->text();
    }else{
        qDebug()<<"save file failed!";
        return false;
    }
    isUnititled = false;
    //获得文件的标准路径
    curFile = QFileInfo(fileName).canonicalFilePath();
    setWindowTitle(curFile);
    path=curFile;
    QString curFile = QFileInfo(fileName).canonicalFilePath();
    QDir currentDir(curFile);
    currentDir.cdUp(); // 切换到上级目录
        QString parentDir = currentDir.canonicalPath(); // 获取上级目录的规范路径
    int number = tree1->root->childCount();
    for(int i = number - 1; i >= 0; i--){
        tree1->root->removeChild(tree1->root->child(i));
    }
    tree1->root->setText(0,parentDir);
    qDebug()<<parentDir;
    QFileInfoList filelist = tree1->allfile(tree1->root,parentDir);
    return true;
}

//槽函数实现-打开文件
bool MainWindow::openFile(const QString &fileName)
{
    QFile f(fileName);
    if(f.open(QIODevice::ReadWrite)){
        QTextStream in(&f);
        QString fcontent = in.readAll();
        createTab();
//        qDebug(tabWidget->tabText(tabWidget->currentIndex());

        curScintilla->setText(fcontent);
        f.close();
    }else{
        qDebug()<<"open file failed!";
        return false;
    }
    curFile = QFileInfo(fileName).canonicalFilePath();
    setWindowTitle(curFile);
    QString curFile = QFileInfo(fileName).canonicalFilePath();
    path = curFile;
    QDir currentDir(curFile);
    currentDir.cdUp(); // 切换到上级目录
    QString parentDir = currentDir.canonicalPath(); // 获取上级目录的规范路径
    int number = tree1->root->childCount();
    for(int i = number - 1; i >= 0; i--){
        tree1->root->removeChild(tree1->root->child(i));
    }
    tree1->root->setText(0,parentDir);
    qDebug()<<parentDir;
    qDebug()<<tabWidget->tabText(tabWidget->currentIndex());
    tabstringtopath[tabWidget->tabText(tabWidget->currentIndex())] = curFile;

    QFileInfoList filelist = tree1->allfile(tree1->root,parentDir);
    return true;

}

void MainWindow::showFindText() {
    // 获取查找文本
    QString searchText = findLineEdit->text();
    // 获取查找文本框的文本
    QString text = curScintilla->text();

    int matchIndex = KMPMatch(text, searchText);

    if (matchIndex != -1) {
        curScintilla->SendScintilla(QsciScintillaBase::SCI_SETSEL, matchIndex, matchIndex + searchText.length());
    }
}

void MainWindow::findNext() {
    // 获取查找文本
    QString searchText = findLineEdit->text();
    // 获取查找文本框的文本
    QString text = curScintilla->text();

    int cursorPosition = curScintilla->SendScintilla(QsciScintillaBase::SCI_GETCURRENTPOS);

    // 从当前光标位置开始查找
    QString remainingText = text.mid(cursorPosition);
    int matchIndex = KMPMatch(remainingText, searchText);

    if (matchIndex != -1) {
        int selectedStart = cursorPosition + matchIndex;
        int selectedEnd = selectedStart + searchText.length();
        curScintilla->SendScintilla(QsciScintillaBase::SCI_SETSEL, selectedStart, selectedEnd);
    }
}

void MainWindow::changeThis() {
    // 获取查找文本
    QString searchText = findLineEdit->text();
    QString to = changeEdit->text();

    // 获取查找文本框的文本
    QString text = curScintilla->text();
    int cursorPosition = curScintilla->SendScintilla(QsciScintillaBase::SCI_GETCURRENTPOS);
    for(int i=cursorPosition-searchText.length();i<=text.length();i++){
        qDebug()<<i;
        QString substring = text.mid(i, searchText.length());
        if(substring==searchText){
            curScintilla->SendScintilla(QsciScintillaBase::SCI_SETSEL, i, i + searchText.length()); // 选中匹配项
            curScintilla->replaceSelectedText(to);
            curScintilla->SendScintilla(QsciScintillaBase::SCI_SETSEL, i, i + searchText.length());
            break;
        }
    }

}

void MainWindow::findLast() {
    // 获取查找文本
    QString searchText = findLineEdit->text();
    QString to = changeEdit->text();

    // 获取查找文本框的文本
    QString text = curScintilla->text();
    int cursorPosition = curScintilla->SendScintilla(QsciScintillaBase::SCI_GETCURRENTPOS);
    for(int i=cursorPosition-searchText.length()-1;i>=0;i--){
        qDebug()<<i;
        QString substring = text.mid(i, searchText.length());
        if(substring==searchText){
            curScintilla->SendScintilla(QsciScintillaBase::SCI_SETSEL, i, i + searchText.length()); // 选中匹配项
            break;
        }
    }

}

void MainWindow::changeText(){
    //替换文本
    // 获取查找文本和替换文本
    QString target = findLineEdit->text();
    QString to = changeEdit->text();

    // 获取QsciScintilla文本内容
    QString text = curScintilla->text();

    // 使用QRegExp进行查找和替换
    QRegExp rx(target);
    text.replace(rx, to);

    // 更新QsciScintilla文本内容
    curScintilla->setText(text);
}

//槽函数实现-字体加粗
void MainWindow::setBold()
{
    //    qDebug()<<tree1->filePath;
    //  QFont font = textEdit->font();

    //  if(font.bold()) {
    //    font.setBold(false);
    //  } else {
    //    font.setBold(true);
    //  }

    //  textEdit->setFont(font);
}

//槽函数实现-字体下划线
void MainWindow::setUnderline()
{
    QFont font = curScintilla->font();

    if(font.underline()) {
        font.setUnderline(false);
    } else {
        font.setUnderline(true);
    }

    curScintilla->setFont(font);
}

//编译的槽函数，编译成功返回0，否则返回1
int MainWindow::compile_file()
{
    if(path.isEmpty())
    {
        // 如果没有路径则需要保存一下才能运行
        this->saveasFile();
    }
    cppCodeRunner *runner = new cppCodeRunner;
    connect(runner, &cppCodeRunner::addCompileErr, this, &MainWindow::displayCompileErr);
    outputTable->clearContents();
    outputTable->setRowCount(0);
    curScintilla->markerDeleteAll();
    int ret = runner->compile(path.at(path.size()-1) == 'c' ? "gcc" : "g++", path, "");
    disconnect(runner, &cppCodeRunner::addCompileErr, this, &MainWindow::displayCompileErr);
    if (!ret){
        if (!isCompile.contains(curScintilla)) isCompile.insert(curScintilla, 1);
        QMessageBox messageBox;
        messageBox.setText("编译成功！");
        messageBox.setStandardButtons(QMessageBox::Ok);
        messageBox.setDefaultButton(QMessageBox::Ok);
        messageBox.exec();

    }
    return ret;
//    QString target = QString("cmd /k %1").arg(demo);
//    system(codec->fromUnicode(target).data());
}

//运行的槽函数：未预先编译会弹窗
void MainWindow::run_file() {
    if (isCompile.contains(curScintilla)) {
        system((path.left(path.lastIndexOf(".")).toStdString()+"&& PAUSE").data());
    }
    else {
        QMessageBox messageBox;
        messageBox.setText("请先编译!");
        messageBox.setStandardButtons(QMessageBox::Ok);
        messageBox.setDefaultButton(QMessageBox::Ok);
        messageBox.exec();
    }
}

void MainWindow::displayCompileErr(cppCompileInfo * info) {
//    qDebug() << info->infoString();
    int rowCnt = outputTable->rowCount();
    outputTable->insertRow(rowCnt);
//    outputTable->setItem(rowCnt, 0, new QTableWidgetItem("dabian"));
    outputTable->setItem(rowCnt, 0, new QTableWidgetItem(info->getFileName()));
    outputTable->setItem(rowCnt, 3, new QTableWidgetItem(info->getType()));
    outputTable->setItem(rowCnt, 4, new QTableWidgetItem(info->getDetail()));
    outputTable->item(rowCnt, 0)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    outputTable->item(rowCnt, 3)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    outputTable->item(rowCnt, 4)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    //can locate to a position
    if (info->getLineNo().at(0) != '-') {
        outputTable->setItem(rowCnt, 1, new QTableWidgetItem(info->getLineNo()));
        outputTable->setItem(rowCnt, 2, new QTableWidgetItem(info->getColNo()));
        outputTable->item(rowCnt, 1)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        outputTable->item(rowCnt, 2)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

        int markerNum = curScintilla->markerDefine(QsciScintilla::RightArrow);
        // 在行前设置标记
        curScintilla->markerAdd(info->getLineNo().toInt()-1, markerNum);
    }

}

void MainWindow::compile_and_run() {
    if (!compile_file())
        run_file();
}

void MainWindow::focusOnPos(int row, int column) {
    curScintilla->setFocus();
    int movRow = outputTable->item(row, 1)->text().toInt();
    int movCol = outputTable->item(row, 2)->text().toInt();
    if (movRow < 0) return ;
    curScintilla->setCursorPosition(movRow-1, movCol-1);
    curScintilla->ensureCursorVisible();
}

MainWindow::~MainWindow()
{
    delete ui;
}


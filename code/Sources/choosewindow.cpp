#include "choosewindow.h"
#include "ui_choosewindow.h"

ChooseWindow::ChooseWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChooseWindow)
{
    // 设置固定大小
    this->setFixedSize(235, 109);
    // 删除最大化按钮
    this->setWindowFlag(Qt::CustomizeWindowHint);
    this->setWindowFlag(Qt::WindowMaximizeButtonHint,false);
    // 自动UI填充
    ui->setupUi(this);

    // 自定义信号-槽连接
    //-->窗口切换有关
    connect(&bTreeWin,SIGNAL(bTreeHidden()),this,SLOT(toChooseWin()));
    connect(&dsWin,SIGNAL(dsHidden()),this,SLOT(toChooseWin()));
    connect(&helpWin,SIGNAL(helpHidden()),this,SLOT(toChooseWin()));
    connect(ui->actBTree,SIGNAL(triggered()),this,SLOT(toBTreeWin()));
    connect(ui->actDS,SIGNAL(triggered()),this,SLOT(toDSWin()));
    connect(ui->actHelp,SIGNAL(triggered()),this,SLOT(toHelpWin()));
}

ChooseWindow::~ChooseWindow() {
    QMessageBox::information(this,"提示","演示结束，祝您生活愉快！");
    delete ui;
}

//---------------> 窗口切换槽 <----/
// 返回主选择界面
void ChooseWindow::toChooseWin() {
    bTreeWin.hide();
    dsWin.hide();
    helpWin.hide();
    this->show();
}
// 进入B-树演示
void ChooseWindow::toBTreeWin() {
    this->hide();
    dsWin.hide();
    helpWin.hide();
    bTreeWin.show();
}
// 进入综合演示
void ChooseWindow::toDSWin() {
    this->hide();
    helpWin.hide();
    bTreeWin.hide();
    dsWin.show();
}
// 进入帮助
void ChooseWindow::toHelpWin() {
    this->hide();
    dsWin.hide();
    bTreeWin.hide();
    helpWin.show();
}

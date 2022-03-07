#include "helpwindow.h"
#include "ui_helpwindow.h"

HelpWindow::HelpWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HelpWindow)
{
    // 设置固定大小
    this->setFixedSize(423, 305);
    // 删除最大化按钮
    this->setWindowFlag(Qt::CustomizeWindowHint);
    this->setWindowFlag(Qt::WindowMaximizeButtonHint,false);
    // 自动UI填充
    ui->setupUi(this);
    // 自定义信号-槽连接
    connect(ui->btnReturn,SIGNAL(clicked()),this,SLOT(backToChooseWin()));
}

HelpWindow::~HelpWindow() {
    delete ui;
}

// 返回主选择界面
void HelpWindow::backToChooseWin() {
    emit helpHidden();
}

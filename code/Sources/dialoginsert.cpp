#include "dialoginsert.h"
#include "ui_dialoginsert.h"

#include "dsstruct.h"
#include "dswindow.h"
#include <QString>
#include <QMessageBox>

DialogInsert::DialogInsert(QWidget *parent, const bool isTree) :
    QDialog(parent),ui(new Ui::DialogInsert),isTree(isTree)
{   // 设置固定大小
    this->setFixedSize(320, 117);

    // 设置图标
    QIcon icon;
    if(isTree)
        icon.addPixmap(QPixmap(":/images/images/TreeInsert.png"));
    else
        icon.addPixmap(QPixmap(":/images/images/LineInsert.png"));
    this->setWindowIcon(icon);

    // 自动UI填充
    ui->setupUi(this);
    // 回车自动插入
    connect(ui->lineEdit, SIGNAL(returnPressed()), ui->btnInsert, SLOT(click()));
}

DialogInsert::~DialogInsert()
{
    delete ui;
}

void DialogInsert::closeEvent(QCloseEvent *event){
    // 获取父窗口指针
    DSWindow *parwind = (DSWindow*)parentWidget();
    // 工具栏重新可以调用
    parwind->setToolBarEnble(true);
    // 将窗口指针设置为NULL
    parwind->setDigNull();
}

void DialogInsert::on_btnInsert_clicked(){
    // 转化数据
    int id = 0, pid = 0;
    Elem data;
    data.dInt = 0;
    data.dStr = "初始字符串";
    QString text = ui->lineEdit->text();
    if(text == "")
        QMessageBox::critical(new QWidget, "错误", "请输入数据");
    else{
        id = text.section(" ",0,0).toInt();
        pid = text.section(" ",1,1).toInt();
        data.dInt = text.section(" ",2,2).toInt();
        data.dStr = text.section(" ",3,-1);
        ui->lineEdit->clear();

        // 插入数据
        DSWindow *parwind = (DSWindow*)parentWidget();
        if(isTree){
            parwind->getTree()->Insert(id,pid,data,parwind->getList());
        }
        else{
            bool result = parwind->getList()->Insert(id,pid,data);
            if(result) // 表中插入成功，则树过时
                parwind->isTreeOutDate = 1;
        }
    }
}

void DialogInsert::on_btnInitial_clicked(){
    DSWindow *parwind = (DSWindow*)parentWidget();
    parwind->initData(isTree);
    if(isTree == 0)
        parwind->isTreeOutDate = 1;
}

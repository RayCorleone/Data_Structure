#include "dialogupdate.h"
#include "ui_dialogupdate.h"

#include "dsstruct.h"
#include "dswindow.h"
#include <QString>
#include <QMessageBox>

DialogUpdate::DialogUpdate(QWidget *parent, const bool isTree) :
    QDialog(parent), ui(new Ui::DialogUpdate), isTree(isTree)
{   // 设置固定大小
    this->setFixedSize(260, 107);

    // 设置图标
    QIcon icon;
    if(isTree)
        icon.addPixmap(QPixmap(":/images/images/TreeUpdate.png"));
    else
        icon.addPixmap(QPixmap(":/images/images/LineUpdate.png"));
    this->setWindowIcon(icon);

    //自动UI填充
    ui->setupUi(this);
    // 回车自动插入
    connect(ui->lineEdit, SIGNAL(returnPressed()), ui->btnUpdate, SLOT(click()));
}

DialogUpdate::~DialogUpdate()
{
    delete ui;
}

void DialogUpdate::closeEvent(QCloseEvent *event){
    // 获取父窗口指针
    DSWindow *parwind = (DSWindow*)parentWidget();
    // 工具栏重新可以调用
    parwind->setToolBarEnble(true);
    // 将窗口指针设置为NULL
    parwind->setDigNull();
}

void DialogUpdate::on_btnUpdate_clicked(){
    // 转化数据
    int id = 0, nid = 0, pid = 0;
    Elem data;
    data.dInt = 0;
    data.dStr = "初始字符串";
    QString text = ui->lineEdit->text();
    if(text == "未查询到ID的数据" || text == "")
        QMessageBox::critical(new QWidget, "错误", "请输入数据");
    else{
        id = ui->spinBox->value();
        nid = text.section(" ",0,0).toInt();
        pid = text.section(" ",1,1).toInt();
        data.dInt = text.section(" ",2,2).toInt();
        data.dStr = text.section(" ",3,-1);

        ui->spinBox->setValue(0);
        ui->lineEdit->clear();

        // 更新数据
        DSWindow *parwind = (DSWindow*)parentWidget();
        if(isTree){
            parwind->getTree()->Update(id,nid,pid,data,parwind->getList());
        }
        else{
            bool result = parwind->getList()->Update(id,nid,pid,data);
            if(result) // 表中更新成功，则树过时
                parwind->isTreeOutDate = 1;
        }
    }
}

// 更新信息
void DialogUpdate::on_spinBox_valueChanged(int id){
    DSWindow *parwind = (DSWindow*)parentWidget();
    LinkList *list = parwind->getList();
    Node* p = list->head->next;
    while(p){
        if(p->id == id)
            break;
        p = p->next;
    }
    if(p){
        QString text = QString::number(p->id);
        text.append(" ").append(QString::number(p->pid));
        text.append(" ").append(QString::number(p->data.dInt));
        text.append(" ").append(p->data.dStr);
        ui->lineEdit->setText(text);
    }
    else{
        ui->lineEdit->setText("未查询到ID的数据");
    }
}

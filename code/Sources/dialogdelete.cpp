#include "dialogdelete.h"
#include "ui_dialogdelete.h"

#include "dsstruct.h"
#include "dswindow.h"
#include <QString>

DialogDelete::DialogDelete(QWidget *parent, const bool isTree) :
    QDialog(parent), ui(new Ui::DialogDelete), isTree(isTree)
{   // 设置固定大小
    this->setFixedSize(260, 107);

    // 设置图标
    QIcon icon;
    if(isTree)
        icon.addPixmap(QPixmap(":/images/images/TreeDelete.png"));
    else
        icon.addPixmap(QPixmap(":/images/images/LineDelete.png"));
    this->setWindowIcon(icon);

    // 自动UI填充
    ui->setupUi(this);
}

DialogDelete::~DialogDelete()
{
    delete ui;
}

void DialogDelete::closeEvent(QCloseEvent *event){
    // 获取父窗口指针
    DSWindow *parwind = (DSWindow*)parentWidget();
    // 工具栏重新可以调用
    parwind->setToolBarEnble(true);
    // 将窗口指针设置为NULL
    parwind->setDigNull();
}

void DialogDelete::on_btnDelete_clicked(){
    // 转化数据
    int id = 0;
    id = ui->spinBox->value();
    ui->spinBox->setValue(0);

    // 删除数据
    DSWindow *parwind = (DSWindow*)parentWidget();
    if(isTree){
        parwind->getTree()->Delete(id,parwind->getList());
    }
    else{
        bool result = parwind->getList()->Delete(id);
        if(result) // 表中删除成功，则树过时
            parwind->isTreeOutDate = 1;
    }
}

void DialogDelete::on_btnClear_clicked(){
    DSWindow *parwind = (DSWindow*)parentWidget();
    Tree *tree = parwind->getTree();
    LinkList *list = parwind->getList();

    // 清空原数据
    tree->root->fChild = tree->Delete(tree->root->fChild);
    emit tree->update();
    int len = list->length;
    for (int i = 0; i < len; i++) {
        Node* p = list->head->next;
        list->Delete(p->id);
    }
    list->iRoot = list->length = 0;
    parwind->isTreeOutDate = 0;
}

// 更新信息
void DialogDelete::on_spinBox_valueChanged(int id){
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

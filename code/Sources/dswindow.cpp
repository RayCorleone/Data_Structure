#include "dswindow.h"
#include "ui_dswindow.h"

#include <QList>

#define TableColumnCount 4 // 表的列数(id-pid-dInt-dStr)

QIcon iconLeaf, iconNode, iconRoot;
QIcon iconFather, iconChild, iconSibling;

DSWindow::DSWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DSWindow)
{
    ///界面UI设置
    this->setFixedSize(751, 492);   // 设置固定大小
    this->setWindowFlag(Qt::CustomizeWindowHint);   // 删除最大化按钮
    this->setWindowFlag(Qt::WindowMaximizeButtonHint,false);
    ui->setupUi(this);  // 自动填充UI

    ///变量准备
    list = new LinkList();
    modelList = new QStandardItemModel(0,TableColumnCount,this);
    selectList = new QItemSelectionModel(modelList);
    ui->tableView->setModel(modelList);
    ui->tableView->setSelectionModel(selectList);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);  // 只读
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);// 选单个
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows); // 选整行
    ui->tableView->setColumnWidth(0,45);
    ui->tableView->setColumnWidth(1,60);
    ui->tableView->setColumnWidth(2,90);
    ui->tableView->setColumnWidth(3,150);
    modelList->setHorizontalHeaderLabels(QStringList() << "--id--" << "--pid--"<<"---dInt---"<<"------dString------");

    tree = new Tree();
    folded = false;
    childSib = false;
    isTreeOutDate = 0;
    modelTree = new QStandardItemModel();
    selectTree = new QItemSelectionModel(modelTree);
    ui->treeView->setModel(modelTree);
    ui->treeView->setSelectionModel(selectTree);
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);  // 只读
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);// 选单个
    ui->treeView->setSelectionBehavior(QAbstractItemView::SelectRows); // 选整行
    modelTree->setHorizontalHeaderLabels(QStringList()<<"-------------id-------------"<<"---dInt---"<<"------dString------");
    ui->treeView->header()->resizeSection(0, 200);
    ui->treeView->header()->resizeSection(1, 70);
    ui->treeView->header()->resizeSection(2, 130);
    ui->treeView->header()->resizeSection(3, 0);

    iconLeaf.addPixmap(QPixmap(":/images/images/Leaf.png"));
    iconNode.addPixmap(QPixmap(":/images/images/Node.png"));
    iconRoot.addPixmap(QPixmap(":/images/images/Root.png"));
    iconFather.addPixmap(QPixmap(":/images/images/Father.png"));
    iconChild.addPixmap(QPixmap(":/images/images/Child.png"));
    iconSibling.addPixmap(QPixmap(":/images/images/Sibling.png"));

    ///自定义信号-槽连接
    //-->工具栏使能有关
    connect(this, SIGNAL(treeUpdate()),this,SLOT(treeToolActivate()));
    //-->视图更新有关
    connect(ui->radioCL,SIGNAL(clicked()),this,SLOT(setDrawTreeMethod()));
    connect(ui->radioCS,SIGNAL(clicked()),this,SLOT(setDrawTreeMethod()));
    connect(tree, SIGNAL(update()), this, SLOT(updateTreeView()));
    connect(list, SIGNAL(update()), this, SLOT(updateTableView()));

    ///视图初始化
    updateTableView();
    updateTreeView();
}

DSWindow::~DSWindow(){
    delete(list);
    delete(tree);
    delete(ui);
}

///////////////////////////////////////////////////////////////////
/// 公共接口函数
///////////////////////////////////////////////////////////////////
// 私有树接口
Tree* DSWindow::getTree(){
    return tree;
}
// 私有表接口
LinkList* DSWindow::getList(){
    return list;
}
// 插入数据初始化
void DSWindow::initData(const bool isTree){
    Elem data;
    data.dInt = 0;
    data.dStr = "初始字符串";

    // 清空原数据
    tree->root->fChild = tree->Delete(tree->root->fChild);
    int len = list->length;
    for (int i = 0; i < len; i++) {
        Node* p = list->head->next;
        list->head->next = p->next;
        delete(p);
    }
    list->iRoot = list->length = 0;

    if(isTree){
        tree->Insert(2,0,data,list);
        tree->Insert(4,2,data,list);
        tree->Insert(7,2,data,list);
        tree->Insert(6,2,data,list);
        tree->Insert(9,4,data,list);
        tree->Insert(8,6,data,list);
        tree->Insert(3,6,data,list);
        tree->Insert(5,8,data,list);
        tree->Insert(1,8,data,list);
    }
    else{
        list->Insert(2,0,data);
        list->Insert(4,2,data);
        list->Insert(7,2,data);
        list->Insert(6,2,data);
        list->Insert(9,4,data);
        list->Insert(8,6,data);
        list->Insert(3,6,data);
        list->Insert(5,8,data);
        list->Insert(1,8,data);
    }
}
// 取消子窗口指针
void DSWindow::setDigNull(){
    listInsertDig = NULL;
    treeInsertDig = NULL;
    listDeleteDig = NULL;
    treeDeleteDig = NULL;
    listUpdateDig = NULL;
    treeUpdateDig = NULL;
}
// 设置工具栏使能
void DSWindow::setToolBarEnble(const bool f){
    ui->actBack->setEnabled(f);
    ui->actToTree->setEnabled(f);
    ui->actTreeInsert->setEnabled(f);
    ui->actTreeUpdate->setEnabled(f);
    ui->actTreeDelete->setEnabled(f);
    ui->actTableInsert->setEnabled(f);
    ui->actTableUpdate->setEnabled(f);
    ui->actTableDelete->setEnabled(f);
    if(f==1 && isTreeOutDate)
        treeToolDisable();
}
// 关闭树操作
void DSWindow::treeToolDisable(){
    ui->actTreeInsert->setEnabled(false);
    ui->actTreeUpdate->setEnabled(false);
    ui->actTreeDelete->setEnabled(false);
}

///////////////////////////////////////////////////////////////////
/// 视图相关槽函数
///////////////////////////////////////////////////////////////////
// 更新表视图
void DSWindow::updateTableView(){
    // 清空原表格
    modelList->removeRows(0, modelList->rowCount());
    // 绘制新表格
    Node *p = list->head;
    for(int i = 0; i < list->length; i++){
        p = p->next;
        QList<QStandardItem*> tempList;
        QStandardItem* temp;
        temp = new QStandardItem(QString::number(p->id));
        temp->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        tempList << temp;
        temp = new QStandardItem(QString::number(p->pid));
        temp->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        tempList << temp;
        temp = new QStandardItem(QString::number(p->data.dInt));
        temp->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        tempList << temp;
        temp = new QStandardItem(p->data.dStr);
        temp->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        tempList << temp;
        // 在表首插入该行
        modelList->insertRow(0, tempList);
    }
    // 更新基本信息
    if(list->IsTree())
        ui->labelTrans->setText("能");
    else
        ui->labelTrans->setText("不能");
    ui->labelTableNum->setText(QString::number(list->length));
}
// 孩子表示法画树
void DSWindow::drawTreeChildList(QStandardItem *fItem, pTree f){
    // 从f的头孩子开始
    pTree child = f->fChild;
    while(child){
        QStandardItem *cItem;
        // 根据有无孩子判断是否是叶子
        if(child->fChild){
            cItem = new QStandardItem(QString::number(child->id).append("->结点"));
            cItem->setIcon(iconNode);
        }
        else{
            cItem = new QStandardItem(QString::number(child->id).append("->叶子"));
            cItem->setIcon(iconLeaf);
        }
        // 为父节点添加孩子结点
        fItem->appendRow(cItem);
        fItem->setChild(cItem->index().row(), 1, new QStandardItem(QString::number(child->data.dInt)));
        fItem->setChild(cItem->index().row(), 2, new QStandardItem(child->data.dStr));
        // 递归孩子结点添加
        drawTreeChildList(cItem, child);
        // 继续下一个孩子
        child = child->nSibling;
    }
}
// 孩子兄弟法画树
void DSWindow::drawTreeChildSibling(QStandardItem *fItem, pTree f, int& i){
    // 左孩子
    if(f->fChild){
        i = i + 1;
        QStandardItem *cItem;
        cItem = new QStandardItem(QString::number(f->fChild->id).append("->").append((QString::number(i)).append("代-孩子")));
        cItem->setIcon(iconChild);
        fItem->appendRow(cItem);
        fItem->setChild(cItem->index().row(), 1, new QStandardItem(QString::number(f->fChild->pid)));
        fItem->setChild(cItem->index().row(), 2, new QStandardItem(QString::number(f->fChild->data.dInt)));
        fItem->setChild(cItem->index().row(), 3, new QStandardItem(f->fChild->data.dStr));
        drawTreeChildSibling(cItem, f->fChild, i);
        i = i - 1;
    }
    // 右兄弟
    if(f->nSibling){
        QStandardItem *cItem;
        cItem = new QStandardItem(QString::number(f->nSibling->id).append("->").append((QString::number(i)).append("代-兄弟")));
        cItem->setIcon(iconSibling);
        fItem->appendRow(cItem);
        fItem->setChild(cItem->index().row(), 1, new QStandardItem(QString::number(f->nSibling->pid)));
        fItem->setChild(cItem->index().row(), 2, new QStandardItem(QString::number(f->nSibling->data.dInt)));
        fItem->setChild(cItem->index().row(), 3, new QStandardItem(f->nSibling->data.dStr));
        drawTreeChildSibling(cItem, f->nSibling, i);
    }
}
// 更新树视图
void DSWindow::updateTreeView(){
    // 清空原视图
    modelTree->removeRows(0, modelTree->rowCount());
    // 绘制新树
    if(childSib){ // 是孩子兄弟表示法
        // 设置表头
        modelTree->setHorizontalHeaderLabels(QStringList()<<"-----------------id-----------------"<<"-pid-"<<"---dInt---"<<"------dString------");
        ui->treeView->header()->resizeSection(0, 250);
        ui->treeView->header()->resizeSection(1, 40);
        ui->treeView->header()->resizeSection(2, 70);
        ui->treeView->header()->resizeSection(3, 130);
        // 是否有树
        if(tree->root->fChild){
            int i = 0;  // 辈分
            QStandardItem *root = new QStandardItem(QString::number(tree->root->fChild->id).append("->0代-祖先"));
            root->setIcon(iconFather);
            modelTree->appendRow(root);
            modelTree->setItem(root->index().row(), 1, new QStandardItem(QString::number(tree->root->fChild->pid)));
            modelTree->setItem(root->index().row(), 2, new QStandardItem(QString::number(tree->root->fChild->data.dInt)));
            modelTree->setItem(root->index().row(), 3, new QStandardItem(tree->root->fChild->data.dStr));
            // 递归画树
            drawTreeChildSibling(root, tree->root->fChild, i);
        }
    }
    else{ // 孩子表示法
        // 设置表头
        modelTree->setHorizontalHeaderLabels(QStringList()<<"-------------id-------------"<<"---dInt---"<<"------dString------");
        ui->treeView->header()->resizeSection(0, 200);
        ui->treeView->header()->resizeSection(1, 70);
        ui->treeView->header()->resizeSection(2, 130);
        ui->treeView->header()->resizeSection(3, 0);
        // 是否有树
        if(tree->root->fChild){
            QStandardItem *root = new QStandardItem(QString::number(tree->root->fChild->id).append("->树根"));
            root->setIcon(iconRoot);
            modelTree->appendRow(root);
            modelTree->setItem(root->index().row(), 1, new QStandardItem(QString::number(tree->root->fChild->data.dInt)));
            modelTree->setItem(root->index().row(), 2, new QStandardItem(tree->root->fChild->data.dStr));
            // 递归画树
            drawTreeChildList(root, tree->root->fChild);
        }
    }
    // 是否展开
    if (folded)
        ui->treeView->collapseAll();
    else
        ui->treeView->expandAll();
    // 更新基本信息
    ui->labelTreeNum->setText(QString::number(tree->size));
}
// 树展开
void DSWindow::on_btnShow_clicked(){
    folded = false;
    ui->treeView->expandAll();
}
// 树收起
void DSWindow::on_btnClose_clicked(){
    folded = true;
    ui->treeView->collapseAll();
}
// 选择树的表示结构
void DSWindow::setDrawTreeMethod(){
    if(ui->radioCL->isChecked())
        childSib = false;
    else if(ui->radioCS->isChecked())
        childSib = true;
    else
        childSib = false;
    updateTreeView();
}

///////////////////////////////////////////////////////////////////
/// 工具栏相关槽函数
///////////////////////////////////////////////////////////////////
// 启用树操作
void DSWindow::treeToolActivate(){
    ui->actTreeInsert->setEnabled(true);
    ui->actTreeUpdate->setEnabled(true);
    ui->actTreeDelete->setEnabled(true);
}
// 表插入操作
void DSWindow::on_actTableInsert_triggered(){
    // 工具栏使能关闭
    setToolBarEnble(0);
    // 创建对话框
    listInsertDig = new DialogInsert(this, 0);
    listInsertDig->setAttribute(Qt::WA_DeleteOnClose);
    listInsertDig->show();
}
// 树插入操作
void DSWindow::on_actTreeInsert_triggered(){
    // 工具栏使能关闭
    setToolBarEnble(0);
    // 创建对话框
    treeInsertDig = new DialogInsert(this, 1);
    treeInsertDig->setAttribute(Qt::WA_DeleteOnClose);
    treeInsertDig->show();
}
// 表修改操作
void DSWindow::on_actTableUpdate_triggered(){
    // 工具栏使能关闭
    setToolBarEnble(0);
    // 创建对话框
    listUpdateDig = new DialogUpdate(this, 0);
    listUpdateDig->setAttribute(Qt::WA_DeleteOnClose);
    listUpdateDig->show();
}
// 树修改操作
void DSWindow::on_actTreeUpdate_triggered(){
    // 工具栏使能关闭
    setToolBarEnble(0);
    // 创建对话框
    treeUpdateDig = new DialogUpdate(this, 1);
    treeUpdateDig->setAttribute(Qt::WA_DeleteOnClose);
    treeUpdateDig->show();
}
// 表删除操作
void DSWindow::on_actTableDelete_triggered(){
    // 工具栏使能关闭
    setToolBarEnble(0);
    // 创建对话框
    listDeleteDig = new DialogDelete(this, 0);
    listDeleteDig->setAttribute(Qt::WA_DeleteOnClose);
    listDeleteDig->show();
}
// 树删除操作
void DSWindow::on_actTreeDelete_triggered(){
    // 工具栏使能关闭
    setToolBarEnble(0);
    // 创建对话框
    treeDeleteDig = new DialogDelete(this, 1);
    treeDeleteDig->setAttribute(Qt::WA_DeleteOnClose);
    treeDeleteDig->show();
}
// 转化操作
void DSWindow::on_actToTree_triggered(){
    tree->InitWithList(list);
    if(list->IsTree()){
        isTreeOutDate = 0;
        emit treeUpdate();
    }
}
// 帮助操作
void DSWindow::on_actHelp_triggered(){
    if(helpDig == NULL)
        helpDig = new DialogHelp(this);
    helpDig->exec();
}
// 退出操作
void DSWindow::on_actBack_triggered(){
    emit dsHidden();
}

#include "btreewindow.h"
#include "ui_btreewindow.h"

#include <QMessageBox>

int keyNum, treeDepth;
QIcon iconBKey, iconBNode, iconBRoot, iconBFail,iconBLeaf;

BTreeWindow::BTreeWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BTreeWindow)
{
    ///界面UI设置
    this->setFixedSize(477, 540);   // 设置固定大小
    this->setWindowFlag(Qt::CustomizeWindowHint);   // 删除最大化按钮
    this->setWindowFlag(Qt::WindowMaximizeButtonHint,false);
    ui->setupUi(this);  // 自动UI填充
    ui->labelNum->setText("0");
    ui->labelDepth->setText("0");
    ui->labelDegree->setText("3");
    ui->lineKey->setValidator(new QIntValidator(this));     // 只接受整数
    ui->lineDegree->setValidator(new QIntValidator(this));  // 只接受整数

    ///变量准备
    folded = false;
    failShow = false;
    btree = new BTree(this, 3);
    modelBTree = new QStandardItemModel();
    ui->treeView->setModel(modelBTree);
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);  // 只读
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);// 选单个
    ui->treeView->setSelectionBehavior(QAbstractItemView::SelectRows); // 选整行
    modelBTree->setHorizontalHeaderLabels(QStringList()<<"-----------范围-----------"<<"----类型----"<<"--键值数--");
    ui->treeView->header()->resizeSection(0, 185);
    ui->treeView->header()->resizeSection(1, 90);
    ui->treeView->header()->resizeSection(2, 60);

    keyNum = treeDepth = 0;
    iconBKey.addPixmap(QPixmap(":/images/images/KeyNode.png"));
    iconBLeaf.addPixmap(QPixmap(":/images/images/BTreeLeaf.png"));
    iconBNode.addPixmap(QPixmap(":/images/images/Node.png"));
    iconBRoot.addPixmap(QPixmap(":/images/images/Root.png"));
    iconBFail.addPixmap(QPixmap(":/images/images/Fail.png"));

    ///自定义信号-槽连接
    connect(btree, SIGNAL(update()), this, SLOT(updateTreeView()));
    connect(ui->btnReturn,SIGNAL(clicked()),this,SLOT(backToChooseWin()));
    connect(ui->lineKey, SIGNAL(returnPressed()), ui->btnInsert, SLOT(click()));
    connect(ui->lineDegree, SIGNAL(returnPressed()), ui->btnReset, SLOT(click()));

    ///视图初始化
    updateTreeView();
}

BTreeWindow::~BTreeWindow(){
    delete(btree);
    delete(ui);
}

///////////////////////////////////////////////////////////////////
/// 视图相关函数
///////////////////////////////////////////////////////////////////
// 展开树
void BTreeWindow::on_btnShow_clicked(){
    folded = false;
    ui->treeView->expandAll();
}
// 收起树
void BTreeWindow::on_btnClose_clicked(){
    folded = true;
    ui->treeView->collapseAll();
}
// 显示失误结点
void BTreeWindow::on_checkBox_stateChanged(int checked){
    failShow = checked;
    updateTreeView();
}
// 递归绘制树
void BTreeWindow::drawBTree(QStandardItem *fItem, BNode *node, int d, const QString l, const QString r){
    // 统计B树信息
    keyNum = keyNum + node->num;
    if(d > treeDepth)
        treeDepth = d;
    // 绘制子节点
    for (int i = 0; i <= node->num; i++){
        BNode *cNode = node->child.at(i);
        // 计算范围
        QString left=(i == 0 ? l:QString::number(node->key.at(i-1)));
        QString right=(i == node->num ? r:QString::number(node->key.at(i)));
        // 结点
        if (cNode){ // 生成结点
            QStandardItem *cItem = new QStandardItem("("+left+","+right+")");
            cItem->setIcon(iconBNode);
            fItem->appendRow(cItem);
            fItem->setChild(cItem->index().row(), 1, new QStandardItem(QString::number(d)+"层子结点"));
            fItem->setChild(cItem->index().row(), 2, new QStandardItem(QString::number(cNode->num)));
            drawBTree(cItem, cNode, d+1, left, right);
        }
        else if(failShow){   // 生成失误结点
            QStandardItem *cItem = new QStandardItem("("+left+","+right+")");
            cItem->setIcon(iconBFail);
            fItem->appendRow(cItem);
            fItem->setChild(cItem->index().row(), 1, 0);
            fItem->setChild(cItem->index().row(), 1, new QStandardItem(QString::number(d+1)+"层失误结点"));
        }
        // 键值
        if (i != node->num){
            QStandardItem *cItem = new QStandardItem(right);
            if(d < treeDepth){
                cItem->setIcon(iconBKey);
                fItem->appendRow(cItem);
                fItem->setChild(cItem->index().row(), 1, new QStandardItem(QString::number(d)+"层键值"));
            }
            else{
                cItem->setIcon(iconBLeaf);
                fItem->appendRow(cItem);
                fItem->setChild(cItem->index().row(), 1, new QStandardItem(QString::number(d)+"层叶子结点"));
            }
        }
    }
}
// 更新树视图
void BTreeWindow::updateTreeView(){
    // 清空视图
    modelBTree->removeRows(0, modelBTree->rowCount());
    keyNum = treeDepth = 0;
    // 绘制新树
    QStandardItem *root = new QStandardItem("(-∞,+∞)");
    root->setIcon(iconBRoot);
    modelBTree->appendRow(root);
    BNode *node = btree->getRoot();
    modelBTree->setItem(root->index().row(), 1, new QStandardItem("0层树根"));
    modelBTree->setItem(root->index().row(), 2, new QStandardItem(QString::number(node->num)));
    // 递归画树
    if(node->num != 0)    drawBTree(root, node, 1, "-∞", "+∞");

    // 是否展开
    if (folded)
        ui->treeView->collapseAll();
    else
        ui->treeView->expandAll();
    // 更新信息
    ui->labelNum->setText(QString::number(keyNum));
    ui->labelDepth->setText(QString::number(treeDepth));
}

///////////////////////////////////////////////////////////////////
/// 按键相关槽函数
///////////////////////////////////////////////////////////////////
// 返回主选择界面
void BTreeWindow::backToChooseWin(){
    emit bTreeHidden();
}
// 插入按键
void BTreeWindow::on_btnInsert_clicked(){
    if (ui->lineKey->hasAcceptableInput()){
        if (!btree->Insert(ui->lineKey->text().toInt()))
            QMessageBox::critical(this, "错误", "键值添加失败");
    }
    ui->lineKey->clear();
}
// 删除按键
void BTreeWindow::on_btnDelete_clicked(){
    if (ui->lineKey->hasAcceptableInput()){
        if (!btree->Delete(ui->lineKey->text().toInt()))
            QMessageBox::critical(this, "错误", "键值删除失败");
    }
    ui->lineKey->clear();
}
// 查找按键
void BTreeWindow::on_btnSearch_clicked(){
    if (ui->lineKey->hasAcceptableInput()){
        Result result = btree->Search(ui->lineKey->text().toInt());
        bool f = result.f;
        if (f)
            QMessageBox::information(this, "查找", "查找成功，键值存在");
        else
            QMessageBox::critical(this, "查找", "查找失败，未找到该键值");
    }
    ui->lineKey->clear();
}
// 清空按键
void BTreeWindow::on_btnClear_clicked(){
    delete(btree);
    ui->labelNum->setText("0");
    ui->labelDepth->setText("0");
    int degree = ui->labelDegree->text().toInt();
    ui->labelDegree->setText(QString::number(degree));

    btree = new BTree(this, degree);
    connect(btree, SIGNAL(update()), this, SLOT(updateTreeView()));
    updateTreeView();
}
// 重设阶数
void BTreeWindow::on_btnReset_clicked(){
    if (ui->lineDegree->hasAcceptableInput()){
        int degree = ui->lineDegree->text().toInt();
        if (degree > 2) { // 符合要求
            ui->labelDegree->setText(QString::number(degree));
            delete(btree);
            btree = new BTree(this, degree);
            connect(btree, SIGNAL(update()), this, SLOT(updateTreeView()));
            updateTreeView();
        }
    }
    ui->lineDegree->clear();
}

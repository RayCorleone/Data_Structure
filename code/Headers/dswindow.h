#ifndef DSWINDOW_H
#define DSWINDOW_H

#include "tree.h"
#include "dsstruct.h"
#include "linklist.h"
#include "dialoghelp.h"
#include "dialoginsert.h"
#include "dialogdelete.h"
#include "dialogupdate.h"

#include <QMainWindow>
#include <QStandardItemModel>
#include <QItemSelectionModel>

namespace Ui {
class DSWindow;
}

class DSWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DSWindow(QWidget *parent = nullptr);
    ~DSWindow();

    bool isTreeOutDate;     // 树是否过时
    Tree* getTree();        // 返回树
    LinkList* getList();    // 返回链表
    void setDigNull();                  // 设置子窗口状态
    void treeToolDisable();             // 关闭树操作
    void setToolBarEnble(const bool f); // 设置工具栏使能
    void initData(const bool isTree);   // 数据初始化
    void drawTreeChildList(QStandardItem *fItem, pTree f);              // 孩子表示法画树
    void drawTreeChildSibling(QStandardItem *fItem, pTree f, int& i);   // 孩子兄弟法画树

private:
    Ui::DSWindow *ui;
    ///线性表相关
    LinkList *list;
    QStandardItemModel *modelList;
    QItemSelectionModel *selectList;
    ///树结构相关
    Tree *tree;
    bool folded;    // 是否展开
    bool childSib;  // 是否孩子兄弟
    QStandardItemModel *modelTree;
    QItemSelectionModel *selectTree;
    ///对话框相关
    DialogHelp *helpDig = NULL;
    DialogInsert *listInsertDig;
    DialogInsert *treeInsertDig;
    DialogDelete *listDeleteDig;
    DialogDelete *treeDeleteDig;
    DialogUpdate *listUpdateDig;
    DialogUpdate *treeUpdateDig;

private slots:
    ///视图更新槽(同时更新label)
    void updateTableView(); // 更新表视图
    void updateTreeView();  // 更新树视图
    ///工具栏槽
    void on_actTableInsert_triggered(); // 表插入
    void on_actTableUpdate_triggered(); // 表更新
    void on_actTableDelete_triggered(); // 表删除
    void on_actTreeInsert_triggered();  // 树插入
    void on_actTreeUpdate_triggered();  // 树更新
    void on_actTreeDelete_triggered();  // 树删除
    void on_actToTree_triggered();      // 转换
    void on_actHelp_triggered();        // 帮助
    void on_actBack_triggered();        // 返回
    ///其他信号槽
    void treeToolActivate();// 启用树操作
    void on_btnShow_clicked();  // 树展开
    void on_btnClose_clicked(); // 树收起
    void setDrawTreeMethod();   // 选择树结构

signals:
    void dsHidden();    // 窗口隐藏
    void treeUpdate();  // 树视图更新
};

#endif // DSWINDOW_H

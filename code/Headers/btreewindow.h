#ifndef BTREEWINDOW_H
#define BTREEWINDOW_H

#include "btree.h"
#include "bnode.h"
#include <QMainWindow>
#include <QStandardItemModel>

namespace Ui {
class BTreeWindow;
}

class BTreeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BTreeWindow(QWidget *parent = nullptr);
    ~BTreeWindow();
    ///画树函数
    void drawBTree(QStandardItem *fItem, BNode *node, int d, const QString l, const QString r);

private:
    Ui::BTreeWindow *ui;
    ///BTree结构相关
    BTree *btree;
    bool folded;
    bool failShow;
    QStandardItemModel *modelBTree;

private slots:
    ///窗口切换槽
    void backToChooseWin(); // 返回选择窗口
    ///视图更新槽(同时更新label)
    void updateTreeView();
    ///按键信号槽
    void on_btnShow_clicked();
    void on_btnClose_clicked();
    void on_btnClear_clicked();
    void on_btnReset_clicked();
    void on_btnInsert_clicked();
    void on_btnDelete_clicked();
    void on_btnSearch_clicked();
    void on_checkBox_stateChanged(int checked);

signals:
    void bTreeHidden();
};

#endif // BTREEWINDOW_H

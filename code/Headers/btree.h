#ifndef BTREE_H
#define BTREE_H

#include <QObject>
#include "bnode.h"

class BTree : public QObject
{
    Q_OBJECT

public:
    explicit BTree(QObject *parent, int degree);
    ~BTree();

    int degree; // 树阶
    BNode *p;   // 获取根节点
    BNode *getRoot();   // 根接口

    bool Insert(const int x);   // 添加键值
    bool Delete(const int x);   // 删除键值
    Result Search(const int x); // 搜索键值

signals:
    void update();
};

#endif // BTREE_H

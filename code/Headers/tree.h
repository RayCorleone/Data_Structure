#ifndef TREE_H
#define TREE_H

#include <QObject>
#include "linklist.h"
#include "dsstruct.h"

class Tree : public QObject
{
    Q_OBJECT
public:
    pTree root;	// 根节点
    int size;	// 节点数

    explicit Tree(QObject *parent = nullptr);
    ~Tree();

    // 将线性表转化为树
    bool Trans(pTree& f, LinkList* list);
    bool InitWithList(LinkList* list);

    // 仅清空树结构:
    pTree Delete(pTree& T);
    pTree DeleteChild(pTree& T);
    // 查找节点:
    pTree Find(const int id, const pTree T) const;
    // 插入节点:
    bool Insert(pTree& node, LinkList* list);
    // 插入新数据节点:
    bool Insert(const int id, const int pid, const Elem data, LinkList* list);
    // 更新节点信息:
    bool Update(const int id, const int nid, const int pid, const Elem data, LinkList* list);
    // 删除节点:
    bool Delete(const int id, LinkList* list);

signals:
    void update();  // 树更新信号
};

#endif // TREE_H

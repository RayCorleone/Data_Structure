#ifndef LINKLIST_H
#define LINKLIST_H

#include <QObject>
#include "dsstruct.h"

class LinkList : public QObject
{
    Q_OBJECT
public:
    Node* head;	// 表头
    int length;	// 表长
    bool iRoot;	// 是否有根节点

    explicit LinkList(QObject *parent = nullptr);
    ~LinkList();

    // 递归计算子树个数
    int CntChild(const int pid) const;
    // 判断能否成为树: 0-不能 1-能
    bool IsTree() const;

    // 检查id重复: 0-不存在 1-存在
    bool Check(const int id);
    // 头部插入新的节点: 0-不成功 1-成功
    bool Insert(const int id, const int pid, const Elem data);
    // 更新节点信息: 0-不成功 1-成功
    bool Update(const int id, const int nid, const int pid, const Elem data);
    // 删除id节点: 0-不成功 1-成功
    bool Delete(const int id);
    // 删除id及衍生节点: 0-不成功 1-成功
    bool DeleteAsTree(const int id);

signals:
    void update();
};

#endif // LINKLIST_H

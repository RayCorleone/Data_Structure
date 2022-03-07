#include "linklist.h"
//#include <QDebug>
#include <QVector>
#include <QMessageBox>

LinkList::LinkList(QObject *parent) : QObject(parent){
    //qDebug() << "Create LinkList!";
    this->iRoot = 0;
    this->length = 0;
    this->head = new Node();
    if (!this->head)
        QMessageBox::critical(new QWidget, "错误", "内存损坏");
    else
        this->head->next = NULL;
}

LinkList::~LinkList(){
    //qDebug() << "Delete LinkList!";
    for (int i = 0; i < this->length; i++) {
        Node* p = this->head->next;
        this->head->next = p->next;
        delete(p);
    }
    this->iRoot = 0;
    this->length = 0;
    delete(this->head);
}

// 递归计算子树个数
int LinkList::CntChild(const int pid) const{
    QVector<int> children;

    // 存储父节点为pid的子节点
    Node* p = this->head->next;
    for (int i = 0; i < this->length; i++) {
        if (p->pid == pid)
            children.push_back(p->id);
        p = p->next;
    }

    // 向下搜索计数
    int cnt = 0;
    for (int i = 0; i < children.size(); i++) {
        cnt = cnt + CntChild(children[i]);
    }
    return cnt + children.size();
}

// 判断能否成为树: 0-不能 1-能
bool LinkList::IsTree() const{
    // 必须有且仅有一个根节点
    if (this->iRoot == 0)
        return 0;

    // 从根节点出发可以涵盖所有节点
    int node = CntChild(0);
    if (node == length)
        return 1;
    else
        return 0;
}

// 检查id重复: 0-不存在 1-存在
bool LinkList::Check(const int id){
    Node* p = this->head->next;
    for (int i = 0; i < this->length; i++) {
        if (p->id == id)
            return 1;
        p = p->next;
    }
    return 0;
}

// 头部插入新的节点: 0-不成功 1-成功
bool LinkList::Insert(const int id, const int pid, const Elem data){
    if (pid == id) {
        QMessageBox::critical(new QWidget, "错误", "ID与父结点ID相同");
        return 0;
    }
    if (pid == 0 && this->iRoot == 1) {
        QMessageBox::critical(new QWidget, "错误", "根节点已经存在");
        return 0;
    }
    if (id == 0) {
        QMessageBox::critical(new QWidget, "错误", "ID值不能为0");
        return 0;
    }
    if (Check(id)) {
        QMessageBox::critical(new QWidget, "错误", "ID已经存在");
        return 0;
    }
    else {
        Node* temp = new Node();
        if (!temp) {
            QMessageBox::critical(new QWidget, "错误", "内存损坏");
            return 0;
        }
        temp->id = id;
        temp->pid = pid;
        temp->data = data;
        temp->next = this->head->next;
        this->head->next = temp;
        this->length = this->length + 1;
        if (pid == 0)
            this->iRoot = 1;

        emit update();
        return 1;
    }
}

// 更新节点信息: 0-不成功 1-成功
bool LinkList::Update(const int id, const int nid, const int pid, const Elem data){
    if (pid == nid) {
        QMessageBox::critical(new QWidget, "错误", "新ID与父结点ID相同");
        return 0;
    }
    if (nid == 0) {
        QMessageBox::critical(new QWidget, "错误", "新ID值不能为0");
        return 0;
    }
    if (id != nid && Check(nid)) {
        QMessageBox::critical(new QWidget, "错误", "新ID已经存在");
        return 0;
    }
    else {
        Node* p = this->head->next;
        for (int i = 0; i < this->length; i++) {
            if (p->id == id) {	// 找到要更改的节点
                // 非根节点要变成根节点，但根节点已经存在
                if (p->pid != 0 && pid == 0 && this->iRoot == 1) {
                    QMessageBox::critical(new QWidget, "错误", "根节点已经存在");
                    return 0;
                }
                // 非根节点要变根节点，根节点不存在，更新信息
                if (p->pid != 0 && pid == 0 && this->iRoot == 0)
                    this->iRoot = 1;
                // 根节点要变非根节点，更新信息
                if (p->pid == 0 && pid != 0)
                    this->iRoot = 0;

                // 更新信息
                //-->与id相关的pid
                Node* t = this->head->next;
                for (int i = 0; i < this->length; i++) {
                    if (t->pid == id)
                        t->pid = nid;
                    t = t->next;
                }
                //-->id\pid\data
                p->id = nid;
                p->pid = pid;
                p->data = data;

                emit update();
                return 1;
            }
            p = p->next;
        }
        QMessageBox::critical(new QWidget, "错误", "ID不存在");
        return 0;
    }
}

// 删除id节点: 0-不成功 1-成功
bool LinkList::Delete(const int id){
    Node* p = this->head;
    for (int i = 0; i < this->length; i++) {
        if (p->next->id == id) {
            Node* t = p->next;
            p->next = t->next;
            if (t->pid == 0)
                this->iRoot = 0;
            delete(t);
            this->length = this->length - 1;

            emit update();
            return 1;
        }
        p = p->next;
    }
    QMessageBox::critical(new QWidget, "错误", "ID不存在");
    return 0;
}

// 删除id及衍生节点: 0-不成功 1-成功
bool LinkList::DeleteAsTree(const int id){
    QVector<int> children;

    // 存储父节点为pid的子节点
    Node* p = this->head->next;
    for (int i = 0; i < this->length; i++) {
        if (p->pid == id)
            children.push_back(p->id);
        p = p->next;
    }

    // 向下清除
    for (int i = 0; i < children.size(); i++)
        DeleteAsTree(children[i]);
    return Delete(id);
}

#include "tree.h"
//#include <QDebug>
#include <QMessageBox>

Tree::Tree(QObject *parent) : QObject(parent){
    //qDebug() << "Create Tree!";
    this->size = 0;
    // root是空根
    this->root = new TreeNode();
    if (!this->root) {
        QMessageBox::critical(new QWidget,"错误","内存损坏");
    }
    else {
        this->root->father = NULL;
        this->root->nSibling = NULL;
        this->root->fChild = NULL;
        this->root->id = 0;
        this->root->pid = 0;
    }
}

Tree::~Tree(){
    //qDebug() << "Delete Tree!";
    this->size = 0;
    Delete(this->root);
}

// 将线性表转化为树
bool Tree::Trans(pTree& f, LinkList* list){ // f为父节点指针
    // 连接父节点和子节点
    Node* p = list->head->next;
    for (int i = 0; i < list->length; i++) {
        if (p->pid == f->id) {
            pTree temp = new TreeNode();
            if (!temp) {
                QMessageBox::critical(new QWidget,"错误","内存损坏");
                return 0;
            }
            this->size = this->size + 1;
            temp->data = p->data;
            temp->id = p->id;
            temp->pid = p->pid;
            temp->fChild = temp->nSibling = NULL;
            temp->father = f;
            temp->nSibling = f->fChild;
            f->fChild = temp;
        }
        p = p->next;
    }
    // 递归子节点
    pTree child = f->fChild;
    while (child != NULL) {
        Trans(child, list);
        child = child->nSibling;
    }
    return 1;
}
bool Tree::InitWithList(LinkList* list){
    // 能转化为树
    if (list->IsTree()) {
        // 清空原来的树
        this->root->fChild = Delete(this->root->fChild);

        Trans(this->root, list);

        emit update();
        return 1;
    }
    // 不能转化为树
    else {
        QMessageBox::critical(new QWidget,"错误","表中数据不能转化为树");
        return 0;
    }

}

// 仅清空树结构:
pTree Tree::DeleteChild(pTree& T){
    if (T) {
        DeleteChild(T->fChild);
        DeleteChild(T->nSibling);
        delete(T);
        this->size = this->size - 1;
        return NULL;
    }
    return NULL;
}
pTree Tree::Delete(pTree& T){
    if (T) {
        DeleteChild(T->fChild);
        delete(T);
        this->size = this->size - 1;
        return NULL;
    }
    emit update();
    return NULL;
}

// 查找节点:
pTree Tree::Find(const int id, const pTree T) const{
    if (T) {
        if (T->id == id)
            return T;
        else {
            pTree l = Find(id, T->fChild);
            pTree r = Find(id, T->nSibling);
            if (l != NULL)
                return l;
            else if (r != NULL)
                return r;
            else
                return NULL;
        }
    }
    else
        return NULL;
}

// 插入节点:
bool Tree::Insert(const int id, const int pid, const Elem data, LinkList* list){
    if (pid == id) {
        QMessageBox::critical(new QWidget, "错误", "ID与父结点ID相同");
        return 0;
    }
    if (id == 0) {
        QMessageBox::critical(new QWidget, "错误", "ID值不能为0");
        return 0;
    }
    if (pid == 0 && (this->root->fChild != NULL)) {
        QMessageBox::critical(new QWidget, "错误", "根节点已经存在");
        return 0;
    }
    if (Find(id, this->root) != NULL) {
        QMessageBox::critical(new QWidget, "错误", "ID已经存在");
        return 0;
    }
    // 寻找父节点
    pTree f = Find(pid, this->root);
    if (f == NULL) {
        QMessageBox::critical(new QWidget, "错误", "父结点ID不存在");
        return 0;
    }
    else {
        pTree temp = new TreeNode();
        if (!temp) {
            QMessageBox::critical(new QWidget, "错误", "内存损坏");
            return 0;
        }
        this->size = this->size + 1;
        temp->id = id;
        temp->pid = pid;
        temp->data = data;
        temp->father = f;
        temp->fChild = temp->nSibling = NULL;
        temp->nSibling = f->fChild;
        f->fChild = temp;

        // 在表中同步插入
        emit update();
        list->Insert(id, pid, data);
        return 1;
    }
}

// 更新节点信息:
bool Tree::Update(const int id, const int nid, const int pid, const Elem data, LinkList* list){
    if (pid == nid) {
        QMessageBox::critical(new QWidget, "错误", "新ID与父结点ID相同");
        return 0;
    }
    if (nid == 0) {
        QMessageBox::critical(new QWidget, "错误", "新ID值不能为0");
        return 0;
    }
    if (id != nid && Find(nid, this->root)) {
        QMessageBox::critical(new QWidget, "错误", "新ID已经存在");
        return 0;
    }
    else {
        pTree p = Find(id, this->root);
        // 找不到id节点
        if (p == NULL) {
            QMessageBox::critical(new QWidget, "错误", "ID不存在");
            return 0;
        }
        // 找到id节点
        pTree f = Find(pid, this->root);
        if (f == NULL) { // 找不到(新的)父节点
            QMessageBox::critical(new QWidget, "错误", "父结点ID不存在");
            return 0;
        }
        else { // 找到(新的)父节点
            // 想改成根节点，但根节点已经存在，且不是这个节点
            if (pid == 0 && this->root->fChild && this->root->fChild->id != id) {
                QMessageBox::critical(new QWidget, "错误", "根节点已经存在");
                return 0;
            }
            // 改数据
            p->data = data;
            // 改pid
            if (p->pid != pid) {
                // 断开旧的连接
                pTree of = p->father;
                if (of->fChild->id == id) // 是头孩子
                    of->fChild = p->nSibling;
                else {	// 不是头孩子
                    pTree c = of->fChild;
                    // 找到孩子
                    while (c->nSibling && c->nSibling->id != id) {
                        c = c->nSibling;
                    }
                    // 跳过孩子连接
                    c->nSibling = p->nSibling;
                }

                // 建立新的连接
                p->father = f;
                p->nSibling = f->fChild;
                f->fChild = p;
            }
            // 改id
            if (id != nid) {
                p->id = nid;
                pTree child = p->fChild;
                while (child) {
                    child->pid = nid;
                    child = child->nSibling;
                }
            }

            // 同步更新表的数据
            emit update();
            list->Update(id, nid, pid, data);
            return 1;
        }
    }
}

// 删除节点:
bool Tree::Delete(const int id, LinkList* list){
    pTree p = Find(id, this->root);
    // 找不到id节点
    if (p == NULL || id == 0) {
        QMessageBox::critical(new QWidget, "错误", "ID不存在");
        return 0;
    }
    // 找到id节点
    else {
        // 更改父节点的连接
        pTree f = p->father;
        if (f->fChild->id == p->id) { // 是头孩子
            f->fChild = p->nSibling;
        }
        else {	// 不是头孩子
            pTree child = f->fChild;
            // 找到孩子
            while (child->nSibling && child->nSibling->id != id) {
                child = child->nSibling;
            }
            // 跳过孩子连接
            child->nSibling = p->nSibling;
        }

        // 删除节点p
        Delete(p);
        list->DeleteAsTree(id);
        emit update();
        return 1;
    }
}

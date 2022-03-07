#include "btree.h"

BTree::BTree(QObject *parent, int drgree)
    : QObject(parent), degree(drgree)
{
    p = new BNode(this);
    new BNode(p);
}

BTree::~BTree(){
    delete(p);
}

BNode * BTree::getRoot(){
    return qobject_cast<BNode *>(p->children().first());
}

Result BTree::Search(const int x){
    return getRoot()->Search(x);
}

bool BTree::Insert(const int x){
    bool result = getRoot()->Insert(x, degree);
    if (result)
        emit update();
    return result;
}

bool BTree::Delete(const int x){
    bool result = getRoot()->Delete(x, degree);
    if (result)
        emit update();
    return result;
}

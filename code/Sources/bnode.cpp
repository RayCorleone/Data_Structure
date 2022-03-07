#include "btree.h"
#include "bnode.h"

BNode::BNode(QObject *parent) : QObject(parent){
    num = 0;
    key.clear();
    child.clear();
    child.append(NULL);
}

BNode::~BNode(){
    key.clear();
    child.clear();
}

Result BNode::Search(const int x){
    // 结点没有键值
    if (num == 0){
        Result result;
        result.f = false;
        result.node = this;
        result.place = 0;
        return result;
    }

    // 结点内二分查找
    int low = 0;
    int high = num - 1;
    while(low <= high){
        int mid = (low + high) / 2;
        if (key.at(mid) == x){
            Result result;
            result.f = true;
            result.node = this;
            result.place = mid;
            return result;
        }
        else if (key.at(mid) > x){
            high = mid - 1;
            continue;
        }
        else if (key.at(mid) < x){
            low = mid + 1;
            continue;
        }
    }

    // 查找下一个结点
    BNode *next;
    next = child.at(low);
    if (next == NULL){  // 没孩子退出
        Result result;
        result.f = false;
        result.node = this;
        result.place = low;
        return result;
    }
    else    // 有孩子继续
        return next->Search(x);
}

bool BNode::Insert(const int x, int m){
    Result result = Search(x);
    if (result.f)
        return false;
    result.node->inKey(x, NULL, NULL, m, result.place);
    return true;
}

bool BNode::inKey(const int x, BNode *cLeft, BNode *cRight, int m, int i){
    if (i == -1){
        // 二分查找
        int low = 0;
        int high = num - 1;

        while(low <= high){
            int mid = (low + high) / 2;
            if (key.at(mid) == x)
                return false;
            else if (key.at(mid) > x){
                high = mid - 1;
                continue;
            }
            else if (key.at(mid) < x){
                low = mid + 1;
                continue;
            }
        }
        i = low;
    }
    // 插入键值
    key.insert(i, x);
    child.insert(i, cLeft);
    child.replace(i + 1, cRight);
    // 键值过多要分割
    num = num + 1;
    if (num == m)
        Split(this, m);

    return true;
}

BNode *BNode::getFather(){
    return qobject_cast<BNode *>(parent());
}

void BNode::Split(BNode *node, int m){
    BNode *sibling;
    BNode *father;

    if (qobject_cast<BTree *>(node->parent()->parent()) != NULL){
        // 该结点为根
        BNode* newRoot = new BNode(node->parent());
        sibling = new BNode(newRoot);
        node->setParent(newRoot);
        father = newRoot;
    }
    else{
        sibling = new BNode(node->parent());
        father = node->getFather();
    }

    sibling->key.append(node->key.mid((node->num+1)/2, m/2));
    sibling->child.clear();
    sibling->child.append(node->child.mid((node->num+1)/2, m/2+1));
    sibling->num = m/2;

    father->inKey((node->key[(node->num-1)/2]), node, sibling, m,-1);
    for (int i = 0; i<m/2+1; i++){
        node->key.removeLast();
        BNode *c = node->child.last();
        if (c)  c->setParent(sibling);
        node->child.removeLast();
    }

    node->num = (node->num - 1) / 2;
}
bool BNode::Delete(const int x, int m){
    Result result = Search(x);
    if (!result.f)
        return false;
    result.node->deleteIndexKey(m, result.place);
    return true;
}

void BNode::deleteIndexKey(int m, int i){
    if (child[0] == NULL)  // 叶子节点
        deleteLeafKey(m, i);
    else{   // 其他
        BNode *leafNode = child[i + 1];
        while (leafNode->child[0] != NULL)
            leafNode = leafNode->child[0];

        key[i] = leafNode->key[0];
        leafNode->deleteLeafKey(m, 0);
    }
}

void BNode::deleteLeafKey(int m, int i){
    int k = key.takeAt(i);
    child.removeAt(i);
    num = num - 1;

    nodeNoFull(this, m, k);
}

void BNode::nodeNoFull(BNode *cur, int m, int k){
    if (cur->num >= (m-1) / 2)
        return;

    if (qobject_cast<BTree *>(cur->parent()->parent()) != NULL)
        return;

    BNode *fatherNode = cur->getFather();

    int myValue;
    if (cur->key.empty())
        myValue = k;
    else
        myValue = cur->key.first();
    int newKey;

    int low = 0;
    int high = fatherNode->num - 1;
    while(low <= high){
        int mid = (low + high) / 2;
        if (fatherNode->key.at(mid) > myValue){
            high = mid - 1;
            continue;
        }
        else if (fatherNode->key.at(mid) <= myValue){
            low = mid + 1;
            continue;
        }
    }

    int index = high;
    BNode *left = ((high != -1) ? fatherNode->child.at(high) : NULL);
    BNode *right = ((low != fatherNode->num) ? fatherNode->child.at(low + 1) : NULL);

    bool leftFull = left && left->num > (m-1) / 2;
    bool rightFull = right && right->num > (m-1) / 2;

    BNode *alive;
    if (leftFull){
        cur->key.push_front(fatherNode->key[index]);
        cur->child.push_front(left->child.takeLast());
        if (cur->child.first())
            cur->child.first()->setParent(cur);
        cur->num = cur->num + 1;

        fatherNode->key[index] = left->key.takeLast();
        left->num = left->num - 1;
    }
    else if (rightFull){
        cur->key.push_back(fatherNode->key[index + 1]);
        cur->child.push_back(right->child.takeFirst());
        if (cur->child.last())
            cur->child.last()->setParent(cur);
        cur->num = cur->num + 1;

        fatherNode->key[index + 1] = right->key.takeFirst();

        right->num = right->num - 1;
    }
    else if (left){
        left->key.append(newKey = fatherNode->key.takeAt(index));
        left->key.append(cur->key);
        left->child.append(cur->child);

        if (cur->child.first()){
            for (auto p = cur->child.cbegin(); p != cur->child.cend(); p++)
                (*p)->setParent(left);
        }

        fatherNode->child.takeAt(index + 1);

        cur->setParent(NULL);

        delete cur;
        alive = left;

        alive->num = alive->key.count();
        fatherNode->num--;

    }
    else if (right){
        cur->key.append(newKey = fatherNode->key.takeAt(index + 1));
        cur->key.append(right->key);
        cur->child.append(right->child);

        if (right->child.first()){
            for (auto p = right->child.cbegin(); p != right->child.cend(); p++)
                (*p)->setParent(cur);
        }

        fatherNode->child.takeAt(index + 2);

        right->setParent(NULL);

        delete right;
        alive = cur;

        alive->num = alive->key.count();
        fatherNode->num--;
    }

    if (!leftFull && !rightFull && qobject_cast<BTree *>(fatherNode->parent()->parent()) != NULL && fatherNode->num == 0){
        alive->setParent(fatherNode->parent());
        fatherNode->setParent(NULL);
        delete fatherNode;
    }
    else
        nodeNoFull(fatherNode, m, newKey);
}

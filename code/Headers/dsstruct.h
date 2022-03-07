#ifndef DSSTRUCT_H
#define DSSTRUCT_H

#include <QString>

// 大类提前声明
class Tree;
class BTree;
class BNode;
class LinkList;

// B-树搜索结果结构体
typedef struct Result {
    bool f;         // 是否找到
    int place;      // 对应位置
    BNode* node;    // 对应结点
}Result;

// 存储数据元素结构体
typedef struct Elem {
    int dInt;
    QString dStr;
}Elem;

// 线性表节点结构体
typedef struct Node {
    int id;		// 该线性表中所有节点的 ID 都唯一
    int pid;	// 表示该节点的父节点，值为 0 表示根，对应线性表中其他节点的 id 值
    Elem data;	// 节点的值，自定义数据类型
    Node* next;	// 指向线性表下一节点
}Node;

// 树结构节点结构体
typedef struct TreeNode {
    int id;		// 该线性表中所有节点的 ID 都唯一
    int pid;	// 表示该节点的父节点，值为 0 表示根，对应线性表中其他节点的 id 值
    Elem data;	// 节点的值，自定义数据类型
    TreeNode* father;	// 父节点指针
    TreeNode* fChild;	// 头孩子指针
    TreeNode* nSibling;	// 右兄弟指针
}TreeNode, * pTree;

#endif // DSSTRUCT_H

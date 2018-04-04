//
// Created by marcin on 03.05.15.
//

#ifndef CUBEVR1_NODE_H
#define CUBEVR1_NODE_H
#include <iostream>
#include <list>
#include <vector>

using namespace std;

template <class T>
class Node {
public:
    int id;
    T key;
    vector<Node*> neighborList; // lista sasiadow
    Node* parent; // ojciec

    Node() {}
    Node(int id, T key, Node* parent) {
        this->id = id;
        this->key = key;
        this->parent = parent;
    }

};


#endif //CUBEVR1_NODE_H

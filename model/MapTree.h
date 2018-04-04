//
// Created by marcin on 03.05.15.
//

#ifndef CUBEVR1_MAPTREE_H
#define CUBEVR1_MAPTREE_H

#include "ModelObject.h"
#include "../lib/Tree.h"

class MapTree {
private:
    Node<ModelObject*> *root; // wskaznik na rootNode
    map<int, Node<ModelObject*> *> nodes; // lista pomocna przy wyszukiwaniu parenta, root Node wiaze wszystkie wierzcholki
    int count; // l. wezlow
public:
    MapTree(Node<ModelObject*> *root);

    int addNode(Node<ModelObject*> *node);
    bool containsNode(int id);
    Node<ModelObject*>* getNodeById(int id);
    void bfs(); // iterativ bfs - to remove

    // recursive dfs algorithm to print map from root
    void dfs();
    void dfsInternal(Node<ModelObject*> *node, bool* discovered);

    int getCount() const {
        return count;
    }
    map<int, Node<ModelObject *> *> getNodes() const;
};


#endif //CUBEVR1_MAPTREE_H

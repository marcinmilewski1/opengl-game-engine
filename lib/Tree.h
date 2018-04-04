//
// Created by marcin on 03.05.15.
//

#ifndef CUBEVR1_TREE_H
#define CUBEVR1_TREE_H

#include <map>
#include "Node.h"
#include <queue>


template <class T>
class Tree {
protected:
    Node<T> *root; // wskaznik na rootNode
    map<int, Node<T> *> nodes; // lista pomocna przy wyszukiwaniu parenta, root Node wiaze wszystkie wierzcholki
public:
    /* root node - id = 0 */
    Tree() { }

    /*Tree(Node<T> root) {
        root = Node<T>(0, nullptr, nullptr);
        nodes.insert(std::pair<int,Node<T>>(root.id, root ));
    }*/

    int addNode(Node<T> *node) {
        // czy jest parent ( czesc dalej do usuniecia )
        if (nodes.count(node->parent->id) == 0) {
            std::cout << "Node with no parent - error" << std::endl;
            return 1;
        }

        // wez parenta podanego wierzcholka
        typename std::map<int, Node<T> *>::iterator it; // iterator do przeszukiwania
        it = nodes.find(node->parent->id);

        // wstaw na liste parenta
        it->second->neighborList.push_back(node);

        // wstaw tez na liste wierzcholkow
        nodes.insert(std::pair<int, Node<T> *>(node->id, node));
        return 0;
    }

    void bfs() {
        bool is_visited[nodes.size()];
        // init is_visited
        for (int i = 0; i < nodes.size(); i++) {
            is_visited[i] = false;
        }
        std::queue<Node<T> *> queue;

        Node<T> *node = root;
        queue.push(node);

        while (!queue.empty()) {
            // 1. jesli wierzcholek odwiedzony
            if (is_visited[node->id] == true) {
                // 1. usun z kolejki
                queue.pop();
                // 2. jesli kolejka pusta zakoncz, jesli nie wez nastepny z kolejki
                if (queue.empty()) break;
                else node = queue.front();
            }

            // 2. Wypisz wierzcholek
            std::cout << "node: " << node->id << std::endl;

            // 3.Dodaj liste sasiedztwa wierzcholka do kolejki
            for (auto entity : node->neighborList) {
                queue.push(entity);
            }

            // 4.Zaznacz jako zaznaczony i usun wierzcholek z kolejki
            is_visited[node->id] = true;
            queue.pop();

            // 5. Wez kolejny z kolejki
            node = queue.front();
        }

    }

    bool containsNode(int id) {
        if (nodes.count(id) > 0) return true;
        else return false;
    }

    Node<T>* getNodeById(int id) {
        typename std::map<int, Node<T> *>::iterator it; // iterator do przeszukiwania
        it = nodes.find(id);

    return it->second;
    }
};


#endif //CUBEVR1_TREE_H

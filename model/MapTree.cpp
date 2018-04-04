//
// Created by marcin on 03.05.15.
//

#include "MapTree.h"



map<int, Node<ModelObject *> *> MapTree::getNodes() const
{
    return nodes;
}
MapTree::MapTree(Node<ModelObject *> *root) {
    this->root = root;
    this->nodes.insert(std::pair<int,Node<ModelObject*>*>(root->id, root));
    count = 1;
}

int MapTree::addNode(Node<ModelObject *>* node) {
        // czy jest parent ( czesc dalej do usuniecia )
        if (nodes.count(node->parent->id) == 0) {
            std::cout << "Node with no parent - error" << std::endl;
            return 1;
        }

        // wez parenta podanego wierzcholka
        std::map<int, Node<ModelObject *>*>::iterator it; // iterator do przeszukiwania
        it = nodes.find(node->parent->id);

        // ustaw macierze lokalnie
        // przesuniecia
        glm::vec3 nodeTranslation = node->key->getTranslation();

        node->key->setTranslation(nodeTranslation);

        //skali
        glm::vec3 nodeScale = node->key->getScale();
       // glm::vec3 dotProduct = glm::vec3(nodeScale.x * parentScale.x, nodeScale.y * parentScale.y, nodeScale.z * parentScale.z);
        node->key->setScale(nodeScale);

        // rotacji
        glm::vec4 nodeRotation = node->key->getRot();

        node->key->setRot(nodeRotation);

        // wstaw na liste dzieci parenta
        it->second->neighborList.push_back(node);

        // wstaw tez na liste wierzcholkow
        nodes.insert(std::pair<int,Node<ModelObject*>*>(node->id,node));

        // zwieksz licznik wierzcholkow
        ++count;
        return 0;
}

void MapTree::bfs() {
    bool is_visited[nodes.size()];
    // init is_visited
    for (int i = 0; i < nodes.size(); i++) {
        is_visited[i] = false;
    }
    std::queue<Node<ModelObject*> *> queue;

    Node<ModelObject*> *node = root;
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

        // 2. Wypisz wierzcholek i wyswietl z przesunieciem
        //std::cout << "node: " << node->id << std::endl;

        glMatrixMode(GL_MODELVIEW);



        glPushMatrix(); // zap. aktualna macierz modelView

        // pomnoz macierz modelwidok przez macierz translacji
        glm::vec3 translation = node->key->getTranslation();
        glTranslatef(translation.x,translation.y,translation.z);


        // pomnoz przez macierz rotacji
        glm::vec4 rotation = node->key->getRot();
        glRotatef(rotation.w, rotation.x, rotation.y, rotation.z);

        // pomnoz przez macierz skali
        glm::vec3 scale = node->key->getScale();
        glScalef(scale.x, scale.y, scale.z);

        // rysuj
        node->key->display();

        glPopMatrix(); // zrzuc ze stosu zap. macierz modelView



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

bool MapTree::containsNode(int id) {
    if (nodes.count(id) > 0) return true;
    else return false;
}

Node<ModelObject *> *MapTree::getNodeById(int id) {
    typename std::map<int, Node<ModelObject *> *>::iterator it; // iterator do przeszukiwania
    it = nodes.find(id);

    return it->second;
}

void MapTree::dfs() {
    // discovered tab
    if (count == 0) return ;;

    bool* tab = new bool[count]();
    // wywoluj rekurencyjnie dfsInternal od roota
    dfsInternal(root, tab);

}

void MapTree::dfsInternal(Node<ModelObject *> *node, bool *discovered) {

    // oznacz wierzcholek jako odkryty
    discovered[node->id] = true;

    // rysuj
    if (node->id != 0) // root -- wyrzucic porownanie
    {
        glMatrixMode(GL_MODELVIEW);

        glPushMatrix(); // zap. aktualna macierz modelView

        // pomnoz macierz modelwidok przez macierz translacji
        glm::vec3 parentTranslation = node->parent->key->getTranslation();
        glm::vec3 nodeTranslation = node->key->getTranslation();
        glm::vec3 translation = parentTranslation + nodeTranslation;
        glTranslatef(translation.x,translation.y,translation.z);

        // translacja dla AABB
        glm::vec3 aabbNodeCenterPointOriginal = node->key->getCenterPointOriginal();
        glm::vec3 aabbNodeMaxPointOriginal = node->key->getMaxOriginal ();
        glm::vec3 aabbNodeMinPointOriginal = node->key->getMinOriginal();
        node->key->setCenterPoint(aabbNodeCenterPointOriginal + translation);
        node->key->setMax (aabbNodeMaxPointOriginal + translation);
        node->key->setMin (aabbNodeMinPointOriginal + translation);

        // pomnoz przez macierz rotacji
        glm::vec4 parentRotation = node->parent->key->getRot();
        glm::vec4 nodeRotation = node->key->getRot();
        glm::vec4 rotation = parentRotation + nodeRotation;
        glRotatef(rotation.w, rotation.x, rotation.y, rotation.z);

        // pomnoz przez macierz skali
        glm::vec3 parentScale = node->parent->key->getScale();
        glm::vec3 nodeScale = node->key->getScale();
        glm::vec3 scale = glm::vec3(nodeScale.x * parentScale.x, nodeScale.y * parentScale.y, nodeScale.z * parentScale.z);
        glScalef(scale.x, scale.y, scale.z);

        // rysuj
        node->key->display();

        glPopMatrix(); // zrzuc ze stosu zap. macierz modelView
    }


    //dla wszystkich sasiadow wywoluj rekurencyjnie
    for (const auto &child : node->neighborList) {
        if (discovered[child->id] == false) dfsInternal(child, discovered);
    }
}

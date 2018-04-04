#include "AabbDetector.h"
#include <iterator>

MapTree *AabbDetector::getMapTree() const
{
    return mapTree;
}

void AabbDetector::setMapTree(MapTree *value)
{
    if (mapTree == nullptr) throw runtime_error("mapTree null in collision detector");
    mapTree = value;
    setVectorOfObjects ();
}

void AabbDetector::detectCollision()
{
    if (objects.size () < 2) return;

    // tablica logiczna, po to by sprawdzac 2 razy dla tych samych obiektow
    bool tab[objects.size ()];
    for (int i = 0; i < objects.size (); i++) {
        tab[i] = false;
    }


    for (int i = 0; i < objects.size(); ++i) {
        ModelObject* obj1 = objects[i];
        tab[i] = true; // ustaw i jako sprawdzony
        // sprawdzaj kazdy z i

        for (int j = 0; j < objects.size (); ++j) {

            // jesli byly sprawdzane idz dalej
            if (tab[i] == true && tab[j] == true) continue;

            ModelObject* obj2 = objects[j];
            // ustaw obj2 jako sprawdzony
            tab[j] = true;

            //Check if Box1's max is greater than Box2's
            //min and Box1's min is less than Box2's max
            glm::vec3 obj1Max = obj1->getMax ();
            glm::vec3 obj1Min = obj1->getMin ();
            glm::vec3 obj2Max = obj2->getMax ();
            glm::vec3 obj2Min = obj2->getMin ();

            cout << "OBJ 1 - id: " << obj1->getId () <<
                    "Max_x: " << obj1Max.x <<
                    "Max_y: " << obj1Max.y <<
                    "Max_z: " << obj1Max.z << endl;

            cout << "OBJ 2 - id: " << obj2->getId () <<
                    "Max_x: " << obj2Max.x <<
                    "Max_y: " << obj2Max.y <<
                    "Max_z: " << obj2Max.z << endl;

            if (
                    // os x
            obj1Max.x > obj2Min.x  &&
            obj1Min.x < obj2Max.x &&
                    // os y
            obj1Max.y > obj2Min.y &&
            obj1Min.y < obj2Max.y &&
                    // os z
            obj1Max.z > obj2Min.z &&
            obj1Min.z < obj2Max.z)
            {
                cout << "KOOOOOLIZJA: " <<
                        "obiekty o id: "
                     << obj1->getId () << " i " << obj2->getId ()
                     << endl;
            }

        }

    }

}

void AabbDetector::setVectorOfObjects()
{
    // ustawienie tablicy obiektow, zamiast mapy
    map<int, Node<ModelObject*> *> nodesMap = mapTree->getNodes ();
    for(map<int, Node<ModelObject*> *>::iterator it = nodesMap.begin (); it != nodesMap.end(); ++it) {
        ModelObject* modelObject = it->second->key;
        // roota nie dodajemy
        if (modelObject->getId () == 0) continue;
        objects.push_back (modelObject);
    }
}

AabbDetector::AabbDetector()
{

}


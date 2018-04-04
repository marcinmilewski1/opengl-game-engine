#ifndef AABBDETECTOR_H
#define AABBDETECTOR_H

#include "MapTree.h"
class AabbDetector
{
private:
    MapTree* mapTree; // wskaznik na mape przechowujaca obiekty
    vector <ModelObject*> objects;

    void setVectorOfObjects();
public:
    AabbDetector();
    MapTree *getMapTree() const;
    void setMapTree(MapTree *value);
    void detectCollision();

};

#endif // AABBDETECTOR_H

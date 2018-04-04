//
// Created by marcin on 06.05.15.
//

#ifndef CUBEVR1_MATERIALCONTAINER_H
#define CUBEVR1_MATERIALCONTAINER_H
#include <string>
#include <map>
#include "Material.h"

using namespace std;
class MaterialContainer {

private:
    map<string, Material*> materialContainer;

public:
    bool containsMaterial(string materialName);
    bool addMaterial(string materialName, Material* material);
    Material* getMaterial(string materialName);
};


#endif //CUBEVR1_MATERIALCONTAINER_H

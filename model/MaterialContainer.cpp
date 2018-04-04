//
// Created by marcin on 06.05.15.
//

#include "MaterialContainer.h"

bool MaterialContainer::containsMaterial(string materialName) {
    if (materialContainer.count(materialName) == 0) return false;
    else return true;
}


bool MaterialContainer::addMaterial(string materialName, Material *material) {
    if (containsMaterial(materialName)) return false;

    materialContainer.insert(pair<string,Material*> (materialName, material));
}

Material *MaterialContainer::getMaterial(string materialName) {

    if (!containsMaterial(materialName)) return nullptr;

    map<string, Material*>::iterator it;
    it = materialContainer.find(materialName);

    return it->second;
}

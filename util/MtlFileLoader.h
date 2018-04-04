//
// Created by marcin on 06.05.15.
//

#ifndef CUBEVR1_MTLFILELOADER_H
#define CUBEVR1_MTLFILELOADER_H


#include <GL/gl.h>
#include "../model/MaterialContainer.h"
#include <SOIL/SOIL.h>

class MtlFileLoader {

private:
MaterialContainer* materialContainer; // po to by nie ladowac materialu ktory juz jest w kontenerze
    bool isMaterialContainerContainsMaterial(string materialName);
    GLuint loadTexture(string texturePath);
public:
    Material* loadMaterial(string mtlFilePath);

    bool parseTexturePath(string temp, string& texturePath);

    void setMaterialContainer(MaterialContainer *materialContainer) {
        MtlFileLoader::materialContainer = materialContainer;
    }
};


#endif //CUBEVR1_MTLFILELOADER_H

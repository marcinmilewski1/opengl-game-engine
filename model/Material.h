//
// Created by marcin on 06.05.15.
//

#ifndef CUBEVR1_MATERIAL_H
#define CUBEVR1_MATERIAL_H
#include <string>
#include <GL/gl.h>

using namespace std;
class Material {

private:
    string name;
    GLuint textureDesc;

public:

    string getName() {
        return name;
    }

    void setName(string name) {
        Material::name = name;
    }

    GLuint getTextureDesc()  {
        return textureDesc;
    }

    void setTextureDesc(GLuint textureDesc) {
        Material::textureDesc = textureDesc;
    }
};


#endif //CUBEVR1_MATERIAL_H

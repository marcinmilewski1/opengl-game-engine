//
// Created by marcin on 25.04.15.
//

#ifndef CUBEVR1_OBJTOVBOPARSER_H
#define CUBEVR1_OBJTOVBOPARSER_H
#define GL_GLEXT_PROTOTYPES

#include <GL/freeglut.h>
#include <GL/gl.h>
#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <vector>
#include <exception>
#include <stdexcept>
#include <glm/detail/type_vec.hpp>
#include <glm/detail/type_vec3.hpp>
#include <glm/detail/type_vec2.hpp>
#include "ObjFileLoader.h"
#include "../model/ModelObject.h"

using namespace std;

class ObjFileLoader {
private:

    ifstream objFile;
    vector<glm::vec3> vertices;
    vector<glm::vec3> normals;
    vector <glm::vec2> textures;

    // indeksy wczytane z faces
    vector<int> vertexIndexes;
    vector<int> normalIndexes;
    vector <int> textureIndexes;

    glm::vec3 parseStringToVec3(string str);
    bool parseFaceStringToIndexes(string str);
    glm::vec2 parseStringToVec2(string str);

public:

    ModelObject* loadObjFile(string filePath);



    vector<glm::vec3> &getVertices() {
        return vertices;
    }


    vector<glm::vec3> &getNormals()  {
        return normals;
    }


    vector<glm::vec2>  &getTextures() {
        return textures;
    }

};


#endif //CUBEVR1_OBJTOVBOPARSER_H

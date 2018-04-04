//
// Created by marcin on 03.05.15.
//

#include "ModelObject.h"



glm::vec3 ModelObject::getCenterPoint() const
{
    return centerPoint;
}

void ModelObject::setCenterPoint(glm::vec3 value)
{
    centerPoint = value;
}

glm::vec3 ModelObject::getMax() const
{
    return max;
}

void ModelObject::setMax(glm::vec3 value)
{
    max = value;
}

glm::vec3 ModelObject::getMin() const
{
    return min;
}

void ModelObject::setMin(glm::vec3 value)
{
    min = value;
}

glm::vec3 ModelObject::getCenterPointOriginal() const
{
    return centerPointOriginal;
}

void ModelObject::setCenterPointOriginal(const glm::vec3 &value)
{
    centerPointOriginal = value;
}

glm::vec3 ModelObject::getMaxOriginal() const
{
    return maxOriginal;
}

void ModelObject::setMaxOriginal(const glm::vec3 &value)
{
    maxOriginal = value;
}

glm::vec3 ModelObject::getMinOriginal() const
{
    return minOriginal;
}

void ModelObject::setMinOriginal(const glm::vec3 &value)
{
    minOriginal = value;
}
void ModelObject::generateAndInitVBO(GLuint type) {
    
    verticesSize = vertices.size() * sizeof(glm::vec3);
    normalsSize = normals.size() * sizeof(glm::vec3);
    texturesSize = textures.size() * sizeof(glm::vec2);

    // utworzneie bufora VBO dla kazdego z wczytaynch obiektow modelObject i przypisanie im deskryptora
    glGenBuffersARB(1, &vboDescriptor);

    // inicjacja bufora VBO dl
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboDescriptor);
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, verticesSize + normalsSize + texturesSize, 0, type);
    glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, verticesSize, &vertices[0]); // // copy vertices starting from 0 offest
    glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, verticesSize, normalsSize, &normals[0]); // copy normals after vertices
    glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, verticesSize + normalsSize, texturesSize, &textures[0]); // // copy uv after normals

    // odbinduj
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}

void ModelObject::display() {

    if (vboDescriptor == 0) return;;
    //bind tekstury
    glBindTexture(GL_TEXTURE_2D, material->getTextureDesc());
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    // aktywacja tekstury
    glClientActiveTexture(textureDescriptor);

    // enable features
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnableClientState( GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
                      // shading mathod: GL_SMOOTH or GL_FLAT
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);      // 4-byte pixel alignment
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    // bind vbo
    glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboDescriptor );

    // vertex arrays opisuja vbo (kolejno wierzch, normalne, kord. uv tekstury), wskazniki traktowane jako offset
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glNormalPointer(GL_FLOAT, 0, (GLvoid *)verticesSize); // normalne za wierzcholkami w buforze itd
    glTexCoordPointer(2, GL_FLOAT, 0, (GLvoid *)(verticesSize +normalsSize));

    // rysuj trojkaty
    glDrawArrays(GL_TRIANGLES, 0, verticesSize);


    /// wylaczanie i odbindowanie aktualnego vbo
    glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_CULL_FACE);
    //unbind vbo
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

    // unbind teksutry
}

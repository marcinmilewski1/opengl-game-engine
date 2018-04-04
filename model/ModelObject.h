//
// Created by marcin on 03.05.15.
//

#ifndef CUBEVR1_MODELOBJECT_H
#define CUBEVR1_MODELOBJECT_H
#define GL_GLEXT_PROTOTYPES

#include <GL/freeglut.h>
#include <GL/gl.h>

#include <string>
#include <glm/glm.hpp>
#include <vector>
#include "Material.h"
#include "Animation.h"

using namespace std;
class ModelObject {
private:
    int id;
    string pathToObjFile;
    glm::vec3 translation; // przesuniecie wzgledem rodzica w strukturze drzewa
    glm::vec3 scale; // skala wzgledna obiektu
    glm::vec4 rot; // rotacja wzgledna obiektu
    GLuint vboDescriptor; // deskryptor bufora vbo dla modelu
    GLuint textureDescriptor; // deskryptor tekstur
    Material* material;
    Animation* animation;

    // TODO: type - static or dynamic ( static na display list, dynamic vertex array)

    vector<glm::vec3> vertices;
    vector<glm::vec3> normals;
    vector <glm::vec2> textures;

    // for AABB
    glm::vec3 centerPointOriginal;
    glm::vec3 maxOriginal;
    glm::vec3 minOriginal;

    glm::vec3 centerPoint;
    glm::vec3 max;
    glm::vec3 min;


    size_t verticesSize;
    size_t normalsSize;
    size_t texturesSize;


public:
    ModelObject(int id, string pathToObjFile, glm::vec3 translation, vector<glm::vec3> vertices,
                vector<glm::vec3> normals, vector<glm::vec2> textures) :               id(id),
                                                                                       pathToObjFile(pathToObjFile),
                                                                                       translation(translation),
                                                                                       vertices(vertices),
                                                                                       normals(normals),
                                                                                       textures(textures) {

    }

    ModelObject () {}

    void generateAndInitVBO(GLuint type);

    void generateTextureDescriptor();


    void display();
    string &getPathToObjFile()  {
        return pathToObjFile;
    }

    void setPathToObjFile(string pathToObjFile) {
        ModelObject::pathToObjFile = pathToObjFile;
    }

    glm::vec3 getTranslation()  {
        return translation;
    }

    void setTranslation(glm::vec3 translation) {
        ModelObject::translation = translation;
    }


    glm::vec3 getScale() const {
        return scale;
    }

    void setScale(glm::vec3 scale) {
        ModelObject::scale = scale;
    }

    glm::vec4 getRot() const {
        return rot;
    }

    void setRot(glm::vec4 rot) {
        ModelObject::rot = rot;
    }

    vector<glm::vec3> &getVertices() {
        return vertices;
    }


    vector<glm::vec3> &getNormals()  {
        return normals;
    }


    vector<glm::vec2>  &getTextures() {
        return textures;
    }


    int getId() {
        return id;
    }

    void setVboDescriptor(GLuint vboDescriptor) {
        this->vboDescriptor = vboDescriptor;
    }


    GLuint getVboDescriptor() {
        return vboDescriptor;
    }

    GLuint* getPointerVboDescriptor() {
        return &vboDescriptor;
    }


    GLuint getTextureDescriptor() const {
        return textureDescriptor;
    }

    GLuint* getPointerTextureDescriptor() {
        return &textureDescriptor;
    }

    void setTexture(GLuint texture) {
        ModelObject::textureDescriptor = texture;
    }


    void setId(int id) {
        ModelObject::id = id;
    }


    size_t getVerticesSize() const {
        return verticesSize;
    }

    size_t getNormalsSize() const {
        return normalsSize;
    }

    size_t getTexturesSize() const {
        return texturesSize;
    }


    Material *getMaterial()  {
        return material;
    }

    void setMaterial(Material *material) {
        ModelObject::material = material;
    }


    Animation *getAnimation() const {
        return animation;
    }

    void setAnimation(Animation *animation) {
        ModelObject::animation = animation;

    }
    void initObjectAnimation() {
        animation->setObjectTranslation(&translation);
        animation->setObjectRotation(&rot);
        animation->setObjectScale(&scale);
    }


    glm::vec3 getCenterPoint() const;
    void setCenterPoint(glm::vec3 value);
    glm::vec3 getMax() const;
    void setMax(glm::vec3 value);
    glm::vec3 getMin() const;
    void setMin(glm::vec3 value);
    glm::vec3 getCenterPointOriginal() const;
    void setCenterPointOriginal(const glm::vec3 &value);
    glm::vec3 getMaxOriginal() const;
    void setMaxOriginal(const glm::vec3 &value);
    glm::vec3 getMinOriginal() const;
    void setMinOriginal(const glm::vec3 &value);
};


#endif //CUBEVR1_MODELOBJECT_H

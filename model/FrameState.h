//
// Created by marcin on 06.06.15.
//

#ifndef CUBEVR1_FRAMESTATE_H
#define CUBEVR1_FRAMESTATE_H
#include <iostream>
#include <glm/glm.hpp>

class FrameState {
private:
    unsigned int frameNo;
    glm::vec3 translation; // przesuniecie
    glm::vec3 scale; // skala
    glm::vec4 rot; // rotacja
public:

    unsigned int getFrameNo() const {
        return frameNo;
    }

    void setFrameNo(unsigned int frameNo) {
        FrameState::frameNo = frameNo;
    }


    glm::vec3 &getTranslation() {
        return translation;
    }

    void setTranslation(glm::vec3 translation) {
        FrameState::translation = translation;
    }

    glm::vec3 &getScale() {
        return scale;
    }

    void setScale(glm::vec3 scale) {
        FrameState::scale = scale;
    }

    glm::vec4 &getRot() {
        return rot;
    }

    void setRot(glm::vec4 rot) {
        FrameState::rot = rot;
    }

    void setTranslationByReference(glm::vec3 &translation) {
        FrameState::translation = translation;
    }

    void setScaleByReference(glm::vec3 &scale) {
        FrameState::scale = scale;
    }

    void setRotByReferece(glm::vec4 &rot) {
        FrameState::rot = rot;
    }

};


#endif //CUBEVR1_FRAMESTATE_H

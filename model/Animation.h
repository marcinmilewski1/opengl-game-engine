//
// Created by marcin on 06.06.15.
//

#ifndef CUBEVR1_ANIMATION_H
#define CUBEVR1_ANIMATION_H

#include "FrameState.h"
#include "vector"
#include <iostream>
#include <map>
#include <glm/glm.hpp>
#include "animationtemplate.h"

class Animation {
private:
    AnimationTemplate* animationTemplate;
    std::vector<int>::iterator framesNoIter; // iterator po wektorze, aby brac nastepne

    bool is_continous; // bedzie sie powtarzac
    bool active;
    int previousKeyFrame;
    int currentFrameNo;
    int nextKeyFrameNo;
    int lastKeyFrameNo;

    // wskazniki na macierze obiektu
    glm::vec3 *objectTranslation;
    glm::vec4* objectRotation;
    glm::vec3* objectScale;

    // roznice miedzy kazda klatka w interpolacji liniowej - obliczane po kazdym cyklu miedzy klatkami kluczowymi
    glm::vec3 deltaTranslation;
    glm::vec4 deltaRotation;
    glm::vec3 deltaScale;

    // pozycja poczatkowa do ew. zapetlenia animacji
    glm::vec3 startObjectTranslation;
    glm::vec4 startObjectRotation;
    glm::vec3 startObjectScale;

    void computeNewDelta();
    void computeMatrices();

public:

    void initAnimation();
    void setNewMatrixes();

    bool isActive() const {
        return active;
    }

    void setActive(bool active) {
        Animation::active = active;
    }

    bool isIs_continous() const {
        return is_continous;
    }

    void setIs_continous(bool is_continous) {
        Animation::is_continous = is_continous;
    }

    int getCurrentFrame() const {
        return currentFrameNo;
    }

    void setCurrentFrame(int currentFrame) {
        Animation::currentFrameNo = currentFrame;
    }

    int getNextFrame() const {
        return nextKeyFrameNo;
    }

    void setNextFrame(int nextFrame) {
        Animation::nextKeyFrameNo = nextFrame;
    }


    glm::vec3 *getObjectTranslation() const;
    void setObjectTranslation(glm::vec3 *value);
    glm::vec4 *getObjectRotation() const;
    void setObjectRotation(glm::vec4 *value);
    glm::vec3 *getObjectScale() const;
    void setObjectScale(glm::vec3 *value);
    AnimationTemplate *getAnimationTemplate() const;
    void setAnimationTemplate(AnimationTemplate *value);
};


#endif //CUBEVR1_ANIMATION_H

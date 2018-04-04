//
// Created by marcin on 06.06.15.
//

#ifndef CUBEVR1_ANIMATIONENGINE_H
#define CUBEVR1_ANIMATIONENGINE_H

#include <vector>
#include <iostream>
#include "Animation.h"

class AnimationEngine {
private:
    std::vector<Animation*> animations;
    int currentFrame; // od 0 do 100

public:
    AnimationEngine() {
        currentFrame = 0;
    }
    void anime();
    void addAnimation(Animation* animation);
};


#endif //CUBEVR1_ANIMATIONENGINE_H

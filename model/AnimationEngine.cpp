//
// Created by marcin on 06.06.15.
//

#include "AnimationEngine.h"

void AnimationEngine::anime() {
    int i = 0;
    ++currentFrame;
    for (Animation* &animation : animations) {
        if(animation->isActive()) {
            animation->setCurrentFrame(currentFrame);
            animation->setNewMatrixes();
//            //DEBUG
//            // 0 - obiekt 1, 1 - obiekt 2
//            std::cout<< i++ << std::endl;

//            std::cout<< "Current Frame: " << currentFrame << std::endl;
//            std::cout<<"Obj Trans: x =" << animation->getObjectTranslation()->x
//                    << ", y = " <<animation->getObjectTranslation()->y
//                    << ", z = " <<animation->getObjectTranslation()->z << std::endl;

//            std::cout<<"Obj Scale: x =" << animation->getObjectScale()->x
//                    << ", y = " <<animation->getObjectScale()->y
//                    << ", z = " <<animation->getObjectScale()->z << std::endl;

//            std::cout<<"Obj Rot: x =" << animation->getObjectRotation()->x
//                    << ", y = " <<animation->getObjectRotation()->y
//                    << ", z = " <<animation->getObjectRotation()->z
//                    << ", angle = " <<animation->getObjectRotation()->w<< std::endl;
        }

        if (currentFrame == 101) {
            //animation->setActive(false);
            currentFrame = 0;
        }
    }
}

void AnimationEngine::addAnimation(Animation *animation) {
    animations.push_back(animation);
}

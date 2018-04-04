#ifndef ANIMATIONPARSER_H
#define ANIMATIONPARSER_H

#include "animationcontainer.h"

class AnimationParser
{
private:
    AnimationContainer* animationContainer;
public:
    AnimationParser() {}
    AnimationParser(AnimationContainer* animationContainer) {
        this->animationContainer = animationContainer;
    }
    void loadAnimationFromFile(string filePath);
    FrameState* parseFrame(std::string temp);
    AnimationContainer *getAnimationContainer() const;
    void setAnimationContainer(AnimationContainer *value);
};

#endif // ANIMATIONPARSER_H

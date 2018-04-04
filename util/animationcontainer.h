#ifndef ANIMATIONCONTAINER_H
#define ANIMATIONCONTAINER_H

#include "../model/animationtemplate.h"

using namespace std;
class AnimationContainer
{
private:
    map<string, AnimationTemplate*> animationContainer;
public:
    AnimationContainer();

    bool containsAnimationTemplate(string templateName);
    bool addTemplate(AnimationTemplate* animationTemplate);
    AnimationTemplate* getAnimationTemplate(string templateName);
};

#endif // ANIMATIONCONTAINER_H

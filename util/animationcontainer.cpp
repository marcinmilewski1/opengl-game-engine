#include "animationcontainer.h"
using namespace std;
AnimationContainer::AnimationContainer()
{

}

bool AnimationContainer::containsAnimationTemplate(string templateName)
{
    if (animationContainer.count(templateName) == 0) return false;
    else return true;
}

bool AnimationContainer::addTemplate(AnimationTemplate *animationTemplate)
{
    if (containsAnimationTemplate(animationTemplate->getName ())) return false;
    // uporzadkuj wektor klatek rosnaco
    animationTemplate->sorting();
    animationContainer.insert(pair<string,AnimationTemplate*> (animationTemplate->getName (), animationTemplate));
}

AnimationTemplate *AnimationContainer::getAnimationTemplate(string templateName)
{
    if (!containsAnimationTemplate(templateName)) return nullptr;

    map<string, AnimationTemplate*>::iterator it;
    it = animationContainer.find(templateName);

    return it->second;
}




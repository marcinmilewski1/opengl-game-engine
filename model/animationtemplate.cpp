#include "animationtemplate.h"
#include <algorithm>    // std::sort

std::string AnimationTemplate::getName() const
{
    return name;
}

void AnimationTemplate::setName(const std::string &value)
{
    name = value;
}

FrameState *AnimationTemplate::getFrame(int frameNo)
{
    std::map<int,FrameState*>::iterator it = frames.find(frameNo);
    return it->second;
}

std::vector<int>::iterator AnimationTemplate::getBeginOfVector()
{
    return framesNo.begin();
}

int AnimationTemplate::getLastFrameNo()
{
    return *(--framesNo.end());
}


AnimationTemplate::AnimationTemplate()
{

}

void AnimationTemplate::sorting()
{
    // uporzadkuj wektor
    std::sort(framesNo.begin(), framesNo.end());
}

void AnimationTemplate::addFrame(FrameState *state)
{
    frames.insert(std::pair<int, FrameState*>(state->getFrameNo(), state));
    // dodaj numer klatki do wektora
    framesNo.push_back(state->getFrameNo());

}


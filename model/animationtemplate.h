#ifndef ANIMATIONTEMPLATE_H
#define ANIMATIONTEMPLATE_H

#include "FrameState.h"

#include <iostream>
#include <string>
#include <map>
#include <vector>
class AnimationTemplate
{
private:
    std::string name;
    std::map<int, FrameState*> frames;
    std::vector<int> framesNo; // uporzadkowane numery klatek kluczowych

public:
    AnimationTemplate();
    void sorting();

    void setFramesNo(std::vector<int> framesNo) {
        this->framesNo = framesNo;
    }
    void addFrame(FrameState* state);

    std::string getName() const;
    void setName(const std::string &value);

    FrameState* getFrame(int frameNo);
    std::vector<int>::iterator getBeginOfVector();
    int getLastFrameNo();
};

#endif // ANIMATIONTEMPLATE_H

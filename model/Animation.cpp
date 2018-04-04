//
// Created by marcin on 06.06.15.
//

#include "Animation.h"

void Animation::setNewMatrixes() {
    // jesli numer obecnej klatki jest wiekszy niz numer nastepnej oblicz nowe delty
    if (currentFrameNo > nextKeyFrameNo) {

        // jesli to ostatnia klatka
        if (currentFrameNo >= lastKeyFrameNo) {
            if (is_continous) {
                *objectTranslation = startObjectTranslation;
                *objectRotation = startObjectRotation;
                *objectScale = startObjectScale;
                initAnimation();
            }
            else {
                active = false;
                return;;
            }
        }
        std::cout << " current > next " << std:: endl;
        ++framesNoIter; // przesun do nastepnej
        previousKeyFrame = nextKeyFrameNo; // nastepna staje sie poprzednia;
        nextKeyFrameNo = *framesNoIter;
        computeNewDelta();
    }

    // oblicz macierze
    computeMatrices();
    //computeNewDelta();
}



glm::vec3 *Animation::getObjectTranslation() const
{
    return objectTranslation;
}

void Animation::setObjectTranslation(glm::vec3 *value)
{
    objectTranslation = value;
}

glm::vec4 *Animation::getObjectRotation() const
{
    return objectRotation;
}

void Animation::setObjectRotation(glm::vec4 *value)
{
    objectRotation = value;
}

glm::vec3 *Animation::getObjectScale() const
{
    return objectScale;
}

void Animation::setObjectScale(glm::vec3 *value)
{
    objectScale = value;
}

AnimationTemplate *Animation::getAnimationTemplate() const
{
    return animationTemplate;
}

void Animation::setAnimationTemplate(AnimationTemplate *value)
{
    animationTemplate = value;
}
void Animation::computeNewDelta() {

    // pobierz nastepna klatke kluczowa
    FrameState *nextKeyFrame = animationTemplate->getFrame(nextKeyFrameNo);
    float dx, dy, dz, angle, span;

    // odleglosc miedzy klatka nastepna a poprzednia
    span = nextKeyFrameNo - previousKeyFrame;
    if (span == 0) throw std::runtime_error("span incorrect ! ! !");

    // przesuniecie w nastepnej klatce (wartosc wzgledna)
    glm::vec3 nextKeyFrameTranslation = nextKeyFrame->getTranslation() + *objectTranslation;

    // rotacja w nastepnej klatce (wartosc bezwzgledna)
    glm::vec4 nextKeyFrameRotation = nextKeyFrame->getRot();

    // skala w nastepnej klatce (wartosc bezwzgledna)
    glm::vec3 nextKeyFrameScale = nextKeyFrame->getScale();

    // oblicz delty dla przesuniecia
    dx = nextKeyFrameTranslation.x - objectTranslation->x;
    dy = nextKeyFrameTranslation.y - objectTranslation->y;
    dz = nextKeyFrameTranslation.z - objectTranslation->z;

    dx /= span;
    dy /= span;
    dz /= span;

    deltaTranslation.x = dx;
    deltaTranslation.y = dy;
    deltaTranslation.z = dz;


    // oblicz delty dla rotacji - jesli wektor obrotu nie zdefiniowany nic nie rob
//    if (objectRotation->x == 0 &&
//        objectRotation->y == 0 &&
//        objectRotation->z == 0) {
//        deltaRotation.w = 0;
//        deltaRotation.x = 0;
//        deltaRotation.y = 0;
//        deltaRotation.z = 0;
//    }
//    else {
        dx = nextKeyFrameRotation.x - objectRotation->x;
        dy = nextKeyFrameRotation.y - objectRotation->y;
        dz = nextKeyFrameRotation.z - objectRotation->z;
        angle = nextKeyFrameRotation.w - objectRotation->w;

        angle /= span;
        if (dx != 0) dx /= span;
        if (dy != 0) dy /= span;
        if (dz != 0) dz /= span;

        deltaRotation.w = angle;
        deltaRotation.x = dx;
        deltaRotation.y = dy;
        deltaRotation.z = dz;
//    }



    //oblicz delty dla skali
    dx = nextKeyFrameScale.x - objectScale->x;
    dy = nextKeyFrameScale.y - objectScale->y;
    dz = nextKeyFrameScale.z - objectScale->z;

    dx /= span;
    dy /= span;
    dz /= span;

    deltaScale.x = dx;
    deltaScale.y = dy;
    deltaScale.z = dz;
}

void Animation::computeMatrices()
{
    // nadpisz macierz obiektu
    *objectScale = *objectScale + deltaScale;
    *objectTranslation = *objectTranslation + deltaTranslation;
    *objectRotation = *objectRotation + deltaRotation;
}

void Animation::initAnimation() {

    // do obliczenia delt od klatki 0 do pierwszej key frame
    framesNoIter = animationTemplate->getBeginOfVector(); // wskazuje na pierwsza klatke kluczowa
    nextKeyFrameNo = *framesNoIter;
    lastKeyFrameNo = animationTemplate->getLastFrameNo();
    previousKeyFrame = 0;
    currentFrameNo = 0;

    if (nextKeyFrameNo == 0) throw std::runtime_error("frame number incorrect");
    computeNewDelta();
}


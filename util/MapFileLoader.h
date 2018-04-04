//
// Created by marcin on 03.05.15.
//
/* mmap file loader
 * mmap: ModelId PathToFile ParentModelId TranslationFromParent
 * ModelId: number
 * PathToFile: string
 * ParentModelId: number
 * TranslationFromParent: number
 */
#ifndef CUBEVR1_MAPFILELOADER_H
#define CUBEVR1_MAPFILELOADER_H
#include <string>
#include <iostream>
#include "../model/MapTree.h"
#include "../model/MaterialContainer.h"
#include "AnimationParser.h"
#include "../model/AnimationEngine.h"
using namespace std;

class MapFileLoader {
private:

    static int mapId ; // kolejne id dla map
    MaterialContainer* materialContainer;
    AnimationParser* animationParser;
    AnimationEngine* animationEngine;

    bool parseMapFileLine(string toParse, string& pathToModelFile, int& modelId, int& parentModelId, glm::vec3 &translationFromParent
    , glm::vec3 &scaleRelative, glm::vec4 &rotRelative, string &animFile);
    string pathToMapFile;
public:

    MapTree* loadMap(string pathToMapFile);


    void setMaterialContainer(MaterialContainer *materialContainer) {
        MapFileLoader::materialContainer = materialContainer;
    }


    AnimationParser *getAnimationParser() const;
    void setAnimationParser(AnimationParser *value);
    AnimationEngine *getAnimationEngine() const;
    void setAnimationEngine(AnimationEngine *value);
};


#endif //CUBEVR1_MAPFILELOADER_H

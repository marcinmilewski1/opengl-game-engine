//
// Created by marcin on 03.05.15.
//

#include <fstream>
#include "MapFileLoader.h"
#include "ObjFileLoader.h"
#include "MtlFileLoader.h"
#include "AnimationParser.h"

MapTree *MapFileLoader::loadMap(string pathToMapFile) {

 fstream mapFile;
 // otwarcie pliku
 mapFile.open(pathToMapFile, std::ifstream::in);
 if(mapFile.fail()) throw runtime_error("File open error.");

 // zapis pliku do bufora
 mapFile.seekg(0, ios::end); // ustawienie wskaznika na koniec pliku
 int length = mapFile.tellg(); // pobierz dlugosc pliku w bajtach
 mapFile.seekg(0, ios::beg); // ustw wsk na poczatek

 char *buffer = new char[length + 1]; // dodaj znak '/n' na koncu bufora dla parsera
 buffer[length] = '/n';

 // read data as a block
 mapFile.read(buffer, length);

 if (mapFile.gcount() != length) throw runtime_error("Can't read entire file.");

 // zamkniecie pliku
 mapFile.close();


 /* ---------- PRZETWARZANIE -----------*/

 // stworz korzen drzewa i drzewo
 ModelObject* rootModelObject = new ModelObject();
 rootModelObject->setId(0);
 rootModelObject->setTranslation(glm::vec3(0,0,0));
 rootModelObject->setScale(glm::vec3(1,1,1));

 Node<ModelObject*>* rootNode = new Node<ModelObject*>(0, rootModelObject, nullptr);
 MapTree* mapTree = new MapTree(rootNode);

 string temp;
 ObjFileLoader objFileLoader;
 MtlFileLoader  mtlFileLoader;
 if (materialContainer == nullptr) {
  throw runtime_error("No material container initialized: " + pathToMapFile);
 }
 else {
  mtlFileLoader.setMaterialContainer(materialContainer);
 }


 // petla po calym pliku
 for (int i = 0; i <= length ; i++) {

  temp.push_back(buffer[i]); // okladaj do tymczasowego stringa

  if (buffer[i] == '\n' && temp.size() != 1) { // jesli koniec wiersza lub pusty wiersz
   string pathToModelFile;
   string pathToAnimFile;
   int modelId;
   int parentModelId;
   glm::vec3 translationFromParent;
   glm::vec3 scaleRelative;
   glm::vec4 rotRelative;
    Animation *animation = new Animation();

   // parsuj linie
   if (parseMapFileLine(temp, pathToModelFile, modelId, parentModelId, translationFromParent, scaleRelative,
                        rotRelative,pathToAnimFile) ==  false) {
    delete mapTree;
    delete rootModelObject;
    delete rootNode;
    throw runtime_error("Can't parse map file , wrong format: " + pathToMapFile);
   }


   // wczytaj model z objFile, ustaw Id i przesuniecie od rodzica, stworz wezel z wczytanym modelem i dodaj go do drzewa
   ModelObject* modelObject = objFileLoader.loadObjFile(pathToModelFile);


   // path materialu to plik z rozszerzeniem .mtl
   string pathToMaterialFile = pathToModelFile;
   string::iterator it = pathToMaterialFile.end();
   *(it-3) = 'm';
   *(it-2) = 't';
   *(it-1) = 'l';

   Material* objectMaterial = mtlFileLoader.loadMaterial(pathToMaterialFile);
   if (objectMaterial == nullptr) {
    throw runtime_error("Can't read object materia.");
   }
   modelObject->setMaterial(objectMaterial);
   int desc = objectMaterial->getTextureDesc();
   if (modelObject == nullptr) throw runtime_error("Can't read Object , wrong obj file.: " + pathToModelFile);
   modelObject->setId(modelId);
   modelObject->setTranslation(translationFromParent);
   modelObject->setScale(scaleRelative);
   modelObject->setRot(rotRelative);


   // wczytaj szablon animacji jesli jest podany
   if (pathToAnimFile != "null") {
       animationParser->loadAnimationFromFile(pathToAnimFile); //wczytuje animacje do kontenera
       // ustawienie animacji dla obietku
      AnimationTemplate* animTempl = animationParser->
              getAnimationContainer()->getAnimationTemplate(pathToAnimFile);

      if (animTempl == nullptr) {
          throw runtime_error("invalid animation template");
      }
      animation->setAnimationTemplate(animTempl);

      // dodaj do obiektu
      modelObject->setAnimation(animation);

      // doda aktualne macierze obiektu do animacji
      modelObject->initObjectAnimation();

      // dodaj do silnika, aktywuj
      animation->setActive(true);
      animation->setIs_continous(false);
      animationEngine->addAnimation (animation);
      animation->initAnimation ();
   }
   else {
       modelObject->setAnimation(nullptr); // w przeciwnym wypadku ustawic wskaznik na null
   }


   // sprawdz czy nie ma bledu w skladni - dodajac do drzewa dziecko, musi byc jego rodzic, pobierz parenta
   if (mapTree->containsNode(parentModelId) == false) {
    delete mapTree;
    delete rootModelObject;
    delete rootNode;
    throw runtime_error("Can't parse map file , wrong format. Parent must be defined.: " + pathToMapFile);
   }
   Node<ModelObject*>* parentNode = mapTree->getNodeById(parentModelId);

   // dodaj model do struktury drzewa
   Node<ModelObject*>* node = new Node<ModelObject*>(modelObject->getId(), modelObject, parentNode);



   mapTree->addNode(node);

   // inicjalizuj vbo modelu
  modelObject->generateAndInitVBO(GL_STATIC_DRAW_ARB);

   temp.clear();
  }

 }
 return mapTree;
}


void MapFileLoader::setAnimationParser(AnimationParser *value)
{
    animationParser = value;
}


AnimationParser *MapFileLoader::getAnimationParser() const
{
    return animationParser;
}



AnimationEngine *MapFileLoader::getAnimationEngine() const
{
    return animationEngine;
}

void MapFileLoader::setAnimationEngine(AnimationEngine *value)
{
    animationEngine = value;
}
bool MapFileLoader::parseMapFileLine(string toParse, string &pathToModelFile, int &modelId, int &parentModelId, glm::vec3 &translationFromParent,glm::vec3 &scaleRelative, glm::vec4 &rotRelative, string &animFile) {
 char tempBuffer[99]; // maksymalna dlugosc sciezki - sciezka do modelu
 char tempBuffer2[99]; // bufor do sciezki anim file
 float x,y,z;
 float xScale, yScale, zScale;
 float xRot, yRot, zRot, angle;
 int no = sscanf(toParse.c_str(), "%d %99s %d : %f,%f,%f : %f,%f,%f : %f,%f,%f,%f : %99s\n",
                 &modelId, &tempBuffer, &parentModelId, &x, &y, &z,
                 &xScale, &yScale, &zScale, &angle, &xRot, &yRot, &zRot, &tempBuffer2 );
 if (no != 14){
  printf("Wrong format !. Cannot parse.\n");
  return false;
 }
 translationFromParent.x = x;
 translationFromParent.y = y;
 translationFromParent.z = z;

 scaleRelative.x = xScale;  // 1 - 100 %
 scaleRelative.y = yScale;
 scaleRelative.z = zScale;

 rotRelative.w = angle;
 rotRelative.x = xRot;
 rotRelative.y = yRot;
 rotRelative.z = zRot;

 string toSave(tempBuffer);
 string toSave2(tempBuffer2);
 pathToModelFile = toSave;
 animFile = toSave2;
 return true;
}

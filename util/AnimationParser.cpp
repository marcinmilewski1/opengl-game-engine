#include "AnimationParser.h"
#include <fstream>
using namespace std;


AnimationContainer *AnimationParser::getAnimationContainer() const
{
    return animationContainer;
}


void AnimationParser::setAnimationContainer(AnimationContainer *value)
{
    animationContainer = value;
}
void AnimationParser::loadAnimationFromFile(string filePath)
{
    // sprawdz czy nie ma szablonu w kontenerze, nazwa szablonu - nazwa wzgl. sciezki
    if (animationContainer->containsAnimationTemplate(filePath)) return;;

    // otwarcie pliku
    fstream animationFile;

    animationFile.open("/home/marcin/ClionProjects/cubeVR1/resources/" + filePath, std::ifstream::in);
    if(animationFile.fail()) throw runtime_error("Mtl file open error.");

    // zapis pliku do bufora
    animationFile.seekg(0, ios::end); // ustawienie wskaznika na koniec pliku
    int length = animationFile.tellg(); // pobierz dlugosc pliku w bajtach
    animationFile.seekg(0, ios::beg); // ustw wsk na poczatek

    char *buffer = new char[length + 1]; // dodaj znak '/n' na koncu bufora dla parsera
    buffer[length] = '/n';

    // read data as a block
    animationFile.read(buffer, length);

    if (animationFile.gcount() != length) throw runtime_error("Can't read entire file.");

    // zamkniecie pliku
    animationFile.close();

    string temp;
    string newNameToSave;
    AnimationTemplate* animationTemplate = new AnimationTemplate();

    for (int i = 0; i <= length; i++) {
        temp.push_back(buffer[i]);


        if(buffer[i] == '\n') {

            if (buffer[i+1] == '\n') {
              // dodaj szablon animacji do kontenera
              animationTemplate->addFrame(parseFrame(temp)); // tu uwaga
              animationTemplate->setName (filePath);
              animationContainer->addTemplate(animationTemplate);
              return;
            }
                // new key frame - parse line
            animationTemplate->addFrame(parseFrame(temp));
            temp.clear();

            }


        }


}

FrameState* AnimationParser::parseFrame(string temp)
{
    // alokacja
    FrameState* frame = new FrameState();
    glm::vec3 translation;
    glm::vec3 scale;
    glm::vec4 rotation;

    int frameNo;
    float x,y,z; // translacja
    float xScale, yScale, zScale;
    float xRot, yRot, zRot, angle;
    int no = sscanf(temp.c_str(), "%d : %f,%f,%f : %f,%f,%f : %f,%f,%f,%f\n", &frameNo,
                    &x, &y, &z,
                    &xScale, &yScale, &zScale, &angle, &xRot, &yRot, &zRot );
    if (no != 11){
     printf("Wrong format !. Cannot parse.\n");
     throw runtime_error("Wrong animation template format");
    }

    translation.x = x;
    translation.y = y;
    translation.z = z;

    scale.x = xScale;
    scale.y = yScale;
    scale.z = zScale;

    rotation.w = angle;
    rotation.x = xRot;
    rotation.y = yRot;
    rotation.z = zRot;

    frame->setScale(scale);
    frame->setTranslation(translation);
    frame->setRot(rotation);
    frame->setFrameNo(frameNo);

    return frame;
}




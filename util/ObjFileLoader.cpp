//
// Created by marcin on 25.04.15.
//

#include "ObjFileLoader.h"
#define VERTICES_PROC_STATUS 0
#define NORMALS_PROC_STATUS 1
#define TEXTURES_PROC_STATUS 2
#define SHADING_PROC_STATUS 3
#define FACES_PROC_STATUS 4
#define OBJECT_PROC_STATUS 5
#define MATERIAL_PROC_STATUS 6
#define GROUP_PROC_STATUS 7
#define INVALID_PROC_STATUS 8

using namespace std;

ModelObject* ObjFileLoader::loadObjFile(string filePath) {

    // wyczysc stan loadera
    vertices.clear();
    normals.clear();
    textures.clear();
    vertexIndexes.clear();
    normalIndexes.clear();
    textureIndexes.clear();

    vector<glm::vec3> tempVertices;
    vector<glm::vec3> tempNormals;
    vector<glm::vec2>  tempTextures;

    float maxX = -1000.0;
    float maxY = -1000.0;
    float maxZ = -1000.0;
    float minX = 1000.0;
    float minY = 1000.0;
    float minZ = 1000.0;

    // otwarcie pliku

    objFile.open(filePath, std::ifstream::in);
    if(objFile.fail()) throw runtime_error("File open error.");

    // zapis pliku do bufora
    objFile.seekg(0, ios::end); // ustawienie wskaznika na koniec pliku
    int length = objFile.tellg(); // pobierz dlugosc pliku w bajtach
    objFile.seekg(0, ios::beg); // ustw wsk na poczatek

    char *buffer = new char[length + 1]; // dodaj znak '/n' na koncu bufora dla parsera
    buffer[length] = '/n';

    // read data as a block
    objFile.read(buffer, length);

    if (objFile.gcount() != length) throw runtime_error("Can't read entire file.");

    // zamkniecie pliku
    objFile.close();


    // ---------------------------------------- przetwarzanie
    int processingStatus = VERTICES_PROC_STATUS;
    string temp;

    // petla po calym pliku
    for (int i = 0 ; i <= length; i++)  {

        temp.push_back(buffer[i]); // okladaj do tymczasowego stringa

        if (buffer[i] == '\n') {    // jesli koniec wiersza
            if (temp.length() < 3) throw runtime_error("Obj file syntax error");

            if (temp[0] == 'v' and temp[1] == ' ') processingStatus = VERTICES_PROC_STATUS;
            else if (temp[0] == 'v' and temp[1] == 't') processingStatus = TEXTURES_PROC_STATUS;
            else if (temp[0] == 'v' and temp[1] == 'n') processingStatus = NORMALS_PROC_STATUS;
            else if (temp[0] == 's' and temp[1] == ' ') processingStatus = SHADING_PROC_STATUS;
            else if (temp[0] == 'f' and temp[1] == ' ') processingStatus = FACES_PROC_STATUS;
            else if (temp[0] == 'o' and temp[1] == ' ') processingStatus = OBJECT_PROC_STATUS;
            else if (temp[0] == 'u') processingStatus = MATERIAL_PROC_STATUS;
            else if (temp[0] == 'g') processingStatus = GROUP_PROC_STATUS;
            else processingStatus = INVALID_PROC_STATUS;

            switch(processingStatus) {
                case VERTICES_PROC_STATUS:
                    tempVertices.push_back(parseStringToVec3(temp));
                    temp.clear(); // czysc tymczasowy string na nastepna linijke
                    break;

                case NORMALS_PROC_STATUS:
                    tempNormals.push_back(parseStringToVec3(temp));
                    temp.clear();
                    break;

                case TEXTURES_PROC_STATUS:
                    tempTextures.push_back(parseStringToVec2(temp));
                    temp.clear();
                    break;

                case FACES_PROC_STATUS:
                    parseFaceStringToIndexes(temp);
                    temp.clear();
                    break;

                default:
                    temp.clear();
                    break;
            }
        }

    }

    // odpowiednie ustawienie wierzcholkow / normalynch i wsp. tekstur zgodnie z faces
    // w pliku obj indeksy numerowane od 1

    // ukladamy wierzcholki po koleji, tak jak w faces
    for (int i = 0 ; i < vertexIndexes.size(); i++) {

        int vertexIndex = vertexIndexes[i];
        glm::vec3 vertex = tempVertices[vertexIndex - 1]; // w pliku obj indeksy sa numerowane od 1
        vertices.push_back(vertex);

        // wylicz max i min dla AABB
        if (vertex.x > maxX) maxX = vertex.x;
        if (vertex.y > maxY) maxY = vertex.y;
        if (vertex.z > maxZ) maxZ = vertex.z;

        if (vertex.x < minX) minX = vertex.x;
        if (vertex.y < minY) minY = vertex.y;
        if (vertex.z < minZ) minZ = vertex.z;
    }

    // ukladamy wspolrzedne UV po koleji, tak jak w faces
    for (int i = 0 ; i < textureIndexes.size(); i++) {
        // ukladamy wierzcholki po koleji, tak jak w faces
        int textureIndex = textureIndexes[i];
        glm::vec2 uv = tempTextures[textureIndex - 1]; // w pliku obj indeksy sa numerowane od 1
        textures.push_back(uv);
    }

    // ukladamy normalne po koleji, tak jak w faces
    for (int i = 0 ; i < normalIndexes.size(); i++) {
        // ukladamy wierzcholki po koleji, tak jak w faces
        int normalIndex = normalIndexes[i];
        glm::vec3 normal = tempNormals[normalIndex - 1]; // w pliku obj indeksy sa numerowane od 1
        normals.push_back(normal);
    }

    delete buffer;

    // wczytanie tekstury

    // wstepnie przesuniecie 0,0,0
    // !! Domysle id modelu - 1
    ModelObject* modelObject =  new ModelObject(1,filePath, glm::vec3(0,0,0), vertices, normals, textures);
    // for AABB
    glm::vec3 maxPoint(maxX, maxY, maxZ);
    glm::vec3 minPoint(minX, minY, minZ);
    glm::vec3 centerPoint;
    centerPoint.x = maxX -((maxX - minX)/2);
    centerPoint.y = maxY -((maxY - minY)/2);
    centerPoint.z = maxZ -((maxZ - minZ)/2);
    modelObject->setMax (maxPoint);
    modelObject->setMin (minPoint);
    modelObject->setCenterPoint (centerPoint);
    // te nie beda sie zmieniac
    modelObject->setMaxOriginal (maxPoint);
    modelObject->setMinOriginal (minPoint);
    modelObject->setCenterPointOriginal (centerPoint);

    return modelObject;
}

glm::vec3 ObjFileLoader::parseStringToVec3(string str) {
    glm::vec3 toReturn;
    bool countStarted = false;

    int startIndex = 0;

    bool is_xParsed = false;
    bool is_yParsed = false;

    for (int i = 0; i < str.length(); i++) {
        if (isdigit(str[i]) or str[i] == '.' or str[i] == '-')  {
            if (countStarted == false) {
                countStarted = true;
                startIndex =i; // zapamietaj indeks poczatku liczby
            }
            else continue;
        }
        else {
            if (countStarted) { //jesli znaleziono wczesniej liczbe - koniec liczby, parsuj
                string temp = str.substr(startIndex, i - startIndex + 1);

                if (!is_xParsed) { // jesli nie sparsowano x - parsuj i ustaw ze sparsowano
                    toReturn.x = (float)atof(temp.c_str());
                    is_xParsed = true;
                    countStarted = false;
                }
                else if (!is_yParsed) {
                    toReturn.y = (float)atof(temp.c_str());
                    is_yParsed = true;
                    countStarted = false;
                }
                else { // parse z
                    toReturn.z = (float)atof(temp.c_str());
                    countStarted = false;
                }

            }
        }
    }
    return toReturn;
}

glm::vec2 ObjFileLoader::parseStringToVec2(string str) {
    glm::vec2 toReturn;
    bool countStarted = false;

    int startIndex = 0;

    bool is_uParsed = false;

    for (int i = 0; i < str.length(); i++) {
        if (isdigit(str[i]) or str[i] == '.' or str[i] == '-')  {
            if (countStarted == false) {
                countStarted = true;
                startIndex =i; // zapamietaj indeks poczatku liczby
            }
            else continue;
        }
        else {
            if (countStarted) { //jesli znaleziono wczesniej liczbe - koniec liczby, parsuj
                string temp = str.substr(startIndex, i - startIndex + 1);

                if (!is_uParsed) { // jesli nie sparsowano u - parsuj i ustaw ze sparsowano
                    toReturn.x = (float)atof(temp.c_str());
                    is_uParsed = true;
                    countStarted = false;
                }
                else  {
                    toReturn.y = (float)atof(temp.c_str());
                    countStarted = false;
                }

            }
        }
    }
    return toReturn;
}

/*
 * W linii 3 wierzcholki opisujace trojkat np. pierwszy 8/11/7
 * 8 - indeks wierzcholka
 * 11 indeks wspolrzednej tekstury
 * 7 - indeks wektora normalnego
 */
bool ObjFileLoader::parseFaceStringToIndexes(string str) {
    std::string vertex1, vertex2, vertex3;
    unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
    int no = sscanf(str.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
    if (no != 9){
        printf("Wrong format !. Cannot parse.\n");
        return false;
    }
    vertexIndexes.push_back(vertexIndex[0]);
    vertexIndexes.push_back(vertexIndex[1]);
    vertexIndexes.push_back(vertexIndex[2]);

    textureIndexes.push_back(uvIndex[0]);
    textureIndexes.push_back(uvIndex[1]);
    textureIndexes.push_back(uvIndex[2]);

    normalIndexes.push_back(normalIndex[0]);
    normalIndexes.push_back(normalIndex[1]);
    normalIndexes.push_back(normalIndex[2]);

    return true;
}


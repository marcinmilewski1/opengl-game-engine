//
// Created by marcin on 06.05.15.
//

#include <fstream>
#include "MtlFileLoader.h"

GLuint MtlFileLoader::loadTexture(string texturePath) {

    string fullPath = "/home/marcin/ClionProjects/cubeVR1/resources/" + texturePath;
    GLuint textureDescriptor = SOIL_load_OGL_texture // load an image file directly as a new OpenGL texture
            (
                    fullPath.c_str(),
                    SOIL_LOAD_AUTO,
                    SOIL_CREATE_NEW_ID,
                    SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
            );

    if (textureDescriptor == 0) {
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
        return 0;
    }

    return textureDescriptor;
}

Material* MtlFileLoader::loadMaterial(string mtlFilePath) {
    // otwarcie pliku
    fstream textureFile;

    textureFile.open(mtlFilePath, std::ifstream::in);
    if(textureFile.fail()) throw runtime_error("Mtl file open error.");

    // zapis pliku do bufora
    textureFile.seekg(0, ios::end); // ustawienie wskaznika na koniec pliku
    int length = textureFile.tellg(); // pobierz dlugosc pliku w bajtach
    textureFile.seekg(0, ios::beg); // ustw wsk na poczatek

    char *buffer = new char[length + 1]; // dodaj znak '/n' na koncu bufora dla parsera
    buffer[length] = '/n';

    // read data as a block
    textureFile.read(buffer, length);

    if (textureFile.gcount() != length) throw runtime_error("Can't read entire file.");

    // zamkniecie pliku
    textureFile.close();

    string temp;
    string newNameToSave;
    Material* material = new Material();

    for (int i = 0; i <= length; i++) {
        temp.push_back(buffer[i]);

        if(buffer[i] == '\n') {
            // if new material
            if (temp[0] == 'n' && temp[1] == 'e' && temp[2] == 'w') {
                // parse materialName name add  to materials container
                char materialNameBuffer[99]; // maksymalna dlugosc nazwy materialu
                char firstString[10];
                int no = sscanf(temp.c_str(), "%10s %99s\n", &firstString, &materialNameBuffer);
                if (no != 2){
                    printf("Wrong mtl format - new material!. Cannot parse.\n");
                    return nullptr;
                }
                string nameMaterial(materialNameBuffer);
                newNameToSave = nameMaterial;
                temp.clear();

                // jesli material jest juz w kontenerze, zwroc lub w p.p dodaj nowy
                if (materialContainer->containsMaterial(materialNameBuffer)) {
                    delete material;
                    material = materialContainer->getMaterial(materialNameBuffer);
                    return material;
                }
                else {
                    material->setName(nameMaterial);
                }
            }

            // parse texture path, load texture, add texture desc to material
            else if (temp[0] == 'm' && temp[1] == 'a' && temp[2] == 'p') {
                // parsuj
                string texturePath;
                 if (parseTexturePath(temp, texturePath) == false) {
                     delete material;
                     throw runtime_error("Can't parse texture path in mtl file");
                 }
                // wczytuj teksture i przypisz deskryptor do materialu
                GLuint textDescriptor = loadTexture(texturePath);
                if (textDescriptor == 0 ) {
                    delete material;
                    return nullptr;
                }
                else {
                material->setTextureDesc(textDescriptor);
                    temp.clear();
                }
            }
            else {
                temp.clear();
            }
        }
    }
    // dodaj material do kontenera
    materialContainer->addMaterial(newNameToSave, material);

    // zwroc material
    return material;
}

bool MtlFileLoader::parseTexturePath(string temp, string& texturePath) {
    char tempBuffer[99]; // maksymalna dlugosc sciezki
    char firstString[10];
    float x,y,z;
    int no = sscanf(temp.c_str(), "%10s %99s\n", &firstString, &tempBuffer );
    if (no != 2){
        printf("Wrong mtl format - map!. Cannot parse.\n");
        return false;
    }
    string toSave(tempBuffer);
    texturePath = toSave;
    return true;
}

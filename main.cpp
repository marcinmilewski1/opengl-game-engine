#define GL_GLEXT_PROTOTYPES

#include <GL/freeglut.h>
#include <GL/gl.h>
#include <iostream>
#include <cmath>
#include <SOIL/SOIL.h>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

#include "model/Camera.h"
#include "util/ObjFileLoader.h"
#include "model/ModelObject.h"
#include "model/MapTree.h"
#include "util/MapFileLoader.h"
#include "model/AnimationEngine.h"
#include "util/animationcontainer.h"
#include "util/AnimationParser.h"
#include "model/AabbDetector.h"

using namespace std;


// prototypes
void display();
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);
void mouseMotion(int x, int y);
void mouse(int button, int state, int x, int y);
void timer(int value);
void idle();

// global variables
Camera camera;
bool key[256];
bool shift_down = false;
bool fps_mode = false;
int viewport_height = 0 ;
int viewport_width = 0;
bool mouse_left_down = false;
bool mouse_right_down = false;

glm::mat4 modelMatrix;
glm::mat4 viewMatrix;
glm::mat4 mvMatrix;

// ustawienia ruchu
const float translationSpeed = 0.05;
const float rotationSpeed = M_PI/180 * 0.2;

MapTree* mapTree = nullptr;
MaterialContainer* materialContainer = nullptr;

AnimationEngine *animationEngine = nullptr;
AnimationContainer *animationContainer = nullptr;
AnimationParser *animationParser = nullptr;
AabbDetector *aabbDetector = nullptr;
// Main Function
int main (int argc, char **argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(640, 480);
    glClearColor(0, 0, 0, 0);                   // specify clear values for the color buffers-  czarne tlo
    glClearStencil(0);                          // specify the clear value for the stencil buffer - stencil ogranicza obszar renderowania
    glClearDepth(0.0f);                         // specify the clear value for the depth buffer -  0 is near, 1 is far
    glutCreateWindow("Kamera fps.");

    // CallBacks
    glutIgnoreKeyRepeat(1);
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutPassiveMotionFunc(mouseMotion);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutIdleFunc(idle);

    glutTimerFunc(1, timer, 0);

    ////////////////////////////////

    string pathToTexture = "/home/marcin/ClionProjects/cubeVR1/resources/road.png";
    GLuint texture = SOIL_load_OGL_texture // load an image file directly as a new OpenGL texture
            (
                    pathToTexture.c_str(),
                    SOIL_LOAD_AUTO,
                    SOIL_CREATE_NEW_ID,
                    SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
            );

    if (texture == 0) {
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
        return false;
    }

    materialContainer = new MaterialContainer();
    animationContainer = new AnimationContainer();
    animationParser = new AnimationParser();
    animationParser->setAnimationContainer (animationContainer);
    // anim engine
    animationEngine = new AnimationEngine();
    aabbDetector = new AabbDetector();

    // wczytaj mape
    MapFileLoader mapFileLoader;
    mapFileLoader.setMaterialContainer(materialContainer);
    mapFileLoader.setAnimationParser(animationParser);
    animationParser->setAnimationContainer(animationContainer);
    mapFileLoader.setAnimationEngine (animationEngine);

    mapTree = mapFileLoader.loadMap("/home/marcin/ClionProjects/cubeVR1/resources/map.mmap");
    if (mapTree == nullptr) {
        printf( "Maploading error:"  );
        return false;
    }

    // ustawienie detektora kolizji
    aabbDetector->setMapTree (mapTree);

    glutMainLoop(); /* Start GLUT event-processing loop */

    return 0;
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); //clear the color buffer and the depth buffer

    glMatrixMode(GL_MODELVIEW);
    // obliczenie macierzy MVP
    camera.refresh(); // odsiwezenie kamery - obliczenie nowej macierzy widoku
    viewMatrix = camera.getViewMatrix();
    modelMatrix = glm::mat4(1.0f);

    mvMatrix = viewMatrix * modelMatrix; // obliczenie nowej macierzy mv

    glLoadMatrixf(glm::value_ptr(mvMatrix)); // zastapienie obecnej macierzy modelview


    /*----------------------------------- RENDERING------------------------------------------ */

    // rysuj wszystkie VBO
    // wyswietl mape
    mapTree->dfs();

    //animuj
    animationEngine->anime();
    std::this_thread::sleep_for(std::chrono::milliseconds(20)); // teoretycznie 50 klatek na sek.
    glutSwapBuffers();
    glutPostRedisplay();

    // sprawdz kolizje
    aabbDetector->detectCollision ();
}

void reshape(int w, int h) {
    viewport_width = w;
    viewport_height = h;

    glViewport(0,0, (GLsizei)w, (GLsizei)h); // ustawienie okna renderowania

    // ustawienie macierzy projekcji
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective((GLdouble) 60.0, (GLdouble) w/h, (GLdouble)0.1, (GLdouble)100.0);
    //projectionMatrix = glm::perspective((double)60.0, double((double)w/(double)h), (double)0.1, (double)100.0); // ustawienie macierzy perspektywy
    glMatrixMode (GL_MODELVIEW); //set the matrix back to model
}

void keyboard(unsigned char pressedKey, int x, int y) {
    if(pressedKey == 27) { // esc - zamknij
        exit(0);
    }

    if (pressedKey == ' ') {
        fps_mode = !fps_mode; // zablokowanie kamery lub odblokowanie jesli spacja

        if (fps_mode) {
            glutSetCursor(GLUT_CURSOR_NONE);
            glutWarpPointer(viewport_width/2, viewport_height/2) ; // ustw. na srodek obszaru renderowania
        }
        else {
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        }
    }

    if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) { // jesli jest focus
        shift_down = true;
    }
    else {
        shift_down = false;
    }

    key[pressedKey] = true;
}

void keyboardUp(unsigned char releasedKey, int x,int y) {
    key[releasedKey] = false;
}

void timer(int value) {
    if (fps_mode) {
        if (key['w'] || key['W']) {
            camera.move(translationSpeed);
        }
        else if (key['s'] || key['S']) {
            camera.move(-translationSpeed);
        }
        else if(key['a'] || key ['A']) {
            camera.strafe(translationSpeed);
        }
        else if(key['d'] || key['D']) {
            camera.strafe(-translationSpeed);
        }
        else if(mouse_left_down) { // jesli klawisz lewy myszy wcisniety
            camera.fly(-translationSpeed);
        }
        else if(mouse_right_down) {
            camera.fly(translationSpeed);
        }
    }
    glutTimerFunc(1, timer, 0);
}

void idle() {
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        if (button == GLUT_LEFT_BUTTON) {
            mouse_left_down = true;
        }
        else if(button == GLUT_RIGHT_BUTTON) {
            mouse_right_down = true;
        }
    }
    else if (state == GLUT_UP) {
        if (button == GLUT_LEFT_BUTTON) {
            mouse_left_down = false;
        }
        else if(button == GLUT_RIGHT_BUTTON) {
            mouse_right_down = false;
        }
    }
}

void mouseMotion(int x, int y) {
    static bool just_warped = false;

    if (just_warped) {
        just_warped = false;
        return ;
    }

    if (fps_mode) {
        int dx = x - viewport_width/2; // aktualna - srodek
        int dy = y - viewport_height/2;

        if(dx) {
            camera.rotateYaw(rotationSpeed * dx);
        }

        if (dy) {
            camera.rotatePitch(rotationSpeed * dy);
        }

        glutWarpPointer(viewport_width / 2, viewport_height/ 2);

        just_warped = true;
    }
}






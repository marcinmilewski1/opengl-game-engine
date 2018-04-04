#ifndef _CUBEVR1_CAMERA_H_
#define _CUBEVR1_CAMERA_H_
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
    private:
        glm::vec3 eye;
        glm::vec3 target;
        glm::vec3 up;

        glm::mat4 viewMatrix;

        float m_x, m_y, m_z;   // Position
        float m_lx, m_ly, m_lz; // Direction vector of where we are looking at
        float m_yaw, m_pitch; // yaw - rotacja wokol osi y, pitch - rotacja wokol osi x
        float m_strafe_lx, m_strafe_lz; // Always 90 degree to direction vector

    public:
        Camera() { init(); }
        ~Camera(){}

        void init();
        void refresh();
        void setPos(float x, float y, float z);
        void getPos(float &x, float &y, float &z);
        void getDirectionVector(float &x, float &y, float &z);
        void setYaw(float angle);
        void setPitch(float angle);
        void createViewMatrix(glm::vec3 eye, glm::vec3 target, glm::vec3 up);
        glm::mat4 getViewMatrix();

        // Navigation
        void move(float trans);
        void strafe(float trans);
        void fly(float incr);
        void rotateYaw(float angle);
        void rotatePitch(float angle);
};


#endif //_CUBEVR1_CAMERA_H_

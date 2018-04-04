#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include "Camera.h"


void Camera::init()
{
    m_yaw = 0.0; // Rotation about Up(Y) Axis
    m_pitch = 0.0;// Rotation about Left(X) Axis
    setPos(0, 0, 0);


}

void Camera::refresh()
{

    m_lx = cos(m_yaw) * cos(m_pitch);
    m_ly = sin(m_pitch);
    m_lz = sin(m_yaw) * cos(m_pitch);

    m_strafe_lx = cos(m_yaw - M_PI_2);
    m_strafe_lz = sin(m_yaw - M_PI_2);

    // ustawiamy wektory
    eye = glm::vec3(m_x, m_y,  m_z); // pozycja kamery
    target = glm::vec3(m_x + m_lx, m_y + m_ly, m_z + m_lz); // pozycja kamery + kierunki
    up = glm::vec3(0.0f, 1.0f,  0.0f);

    createViewMatrix(eye,target,up);

}

void Camera::setPos(float x, float y, float z)
{
    m_x = x;
    m_y = y;
    m_z = z;

    refresh();
}

void Camera::getPos(float &x, float &y, float &z)
{
    x = m_x;
    y = m_y;
    z = m_z;
}

void Camera::getDirectionVector(float &x, float &y, float &z)
{
    x = m_lx;
    y = m_ly;
    z = m_lz;
}

void Camera::move(float trans)
{
    float lx = cos(m_yaw)*cos(m_pitch);
    float ly = sin(m_pitch);
    float lz = sin(m_yaw)*cos(m_pitch);

    m_x = m_x + trans *lx;
    m_y = m_y + trans *ly;
    m_z = m_z + trans *lz;

    refresh();
}

void Camera::strafe(float trans)
{
    m_x = m_x + trans *m_strafe_lx;
    m_z = m_z + trans *m_strafe_lz;

    refresh();
}

void Camera::fly(float incr)
{
    m_y = m_y + incr;

    refresh();
}

void Camera::rotateYaw(float angle)
{
    m_yaw += angle;

    refresh();
}

void Camera::rotatePitch(float angle)
{
    const float limit = 89.0 * M_PI / 180.0;

    m_pitch += angle;

    if(m_pitch < -limit)
        m_pitch = -limit;

    if(m_pitch > limit)
        m_pitch = limit;

    refresh();
}

void Camera::setYaw(float angle)
{
    m_yaw = angle;

    refresh();
}

void Camera::setPitch(float angle)
{
    m_pitch = angle;

    refresh();
}

glm::mat4 Camera::getViewMatrix() {
    return viewMatrix;
}

void Camera::createViewMatrix(glm::vec3 eye, glm::vec3 target, glm::vec3 up) {
    glm::vec3 vz = glm::normalize(eye - target); // the forward vector - os z
    glm::vec3 vx = glm::normalize(glm::cross(up, vz)); // the right vector - os x
    glm::vec3 vy = glm::cross(vz, vx); // the up vector - os y


    // zbuduj macierz modelu
    glm::mat4 modelMatrix = glm::mat4(glm::vec4(vx,0),
                           glm::vec4(vy, 0),
                           glm::vec4(vz, 0),
                           glm::vec4(eye, 1)
    );

    viewMatrix = glm::inverse(modelMatrix); // macierz widoku jest macierza odwrotna do zbudowanej, I = VM , V = M^-1


}
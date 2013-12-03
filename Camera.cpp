#include <math.h>

#include <stdlib.h>
#ifdef OSX
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

#include "Camera.h"
#include "algebra3.h"

static float PI = 4*atan(1);

vec3 Camera::location() {
    // Point to store location (x, y, z)
    vec3 location;
    // Convert hAngle and vAngle to radians
    float hAngleRads = hAngle * (PI / 180);
    float vAngleRads = vAngle * (PI / 180);
    // Apply vertical angle movement (rotation about z-axis)
    location[VY] = radius * cos(vAngleRads);
    location[VZ] = radius * sin(vAngleRads);
    // Apply horizontal angle movement (rotation about y-axis)
    float old_x = location[VX];
    float old_y = location[VY];
    float cos_term = cos(hAngleRads);
    float sin_term = sin(hAngleRads);
    location[VX] = cos_term * old_x + sin_term * old_y;
    location[VY] = -sin_term * old_x + cos_term * old_y;
    return location;
}

void Camera::rotateH(float angle) {
    if (hAngle > 360) {
        hAngle -= 360;
    } else if (hAngle < 0) {
        hAngle += 360;
    }
    hAngle += angle;
}

void Camera::rotateV(float angle) {
    // Don't let the vAngle leave the range [-85, 85]
    vAngle = fmin(85, fmax(-85, vAngle + angle));
}

void Camera::zoom(float amount) {
    radius = fmax(0.1f, radius + amount);
}

void Camera::update() {
    vec3 eye_loc = location();
    gluLookAt(eye_loc[VX], eye_loc[VY], eye_loc[VZ],
              0, 0, 0,
              0, 0, 1);
}

vec3 Camera::forward() {
    return ( -1 * vec3(location()).normalize());
}

vec3 Camera::up() {
    vec3 forward = this->forward();
    vec3 yAxis(0, 0, 1);
    vec3 right = forward ^ yAxis;
    return (right ^ forward).normalize();
}

vec3 Camera::right() {
    vec3 forward = this->forward();
    vec3 yAxis(0, 0, 1);
    return (forward ^ yAxis).normalize();
}

float Camera::getRadius() {
    return radius;
}

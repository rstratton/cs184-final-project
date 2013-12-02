#ifndef CAMERA_H
#define CAMERA_H

#include "algebra3.h"

// The camera's position is specified in polar coordinates.  It always looks
// at the origin (this can be changed in the update method; currently values
// are hard-coded to look at the origin).  The up-vector is always (0, 1, 0).
// Moving the camera is done by rotating it laterally via rotateH, or vertically
// via rotateV, or by zooming in/out (i.e. changing its positional radius)
class Camera {
    private:
        // Represent position in spherical coordinates
        // Horizontal angle, vertical angle, and radius
        float hAngle, vAngle, radius;

    public:
        Camera() { hAngle = 0; vAngle = 0; radius = 1; }
        Camera(float h, float v, float r) {
            hAngle = h; vAngle = v; radius = r;
        }

        // Get eye location in (x, y, z)
        vec3 location();
        // Perform rotation about the y-axis
        void rotateH(float angle);
        // Perform rotation out of the x/z-plane
        void rotateV(float angle);
        // Modify the radius; negative zoom indicates zooming in (i.e.
        // decreasing the radius); positive zoom indications zooming out (i.e.
        // increasing the radius).
        void zoom(float amount);
        // Call the OpenGL functions to update the camera position
        void update();
        // Get the unit vector pointing in the direction of the camera view
        vec3 forward();
        // Get the unit vector pointing up, perpendicular to the forward vector
        vec3 up();
        // Get the unit vector pointing right, perpendicular to the forward vector
        vec3 right();
        float getRadius();
};

#endif

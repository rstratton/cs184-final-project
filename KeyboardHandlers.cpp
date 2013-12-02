#include <GLUT/glut.h>
#include "KeyboardHandlers.h"
#include "Camera.h"
#include "Renderer.h"
#include "algebra3.h"

KeyboardHandlers::KeyboardHandlers(Camera* c, vec3* pos, Renderer* r) {
    camera = c;
    objectPosition = pos;
    renderer = r;
}

void KeyboardHandlers::basicKeyHandler(unsigned char key, int x, int y) {
    switch (key) {
        case ' ':
            exit(0);
            break;
        case '=':
            camera->zoom(-0.2f * camera->getRadius());
            break;
        case '-':
            camera->zoom(0.25f * camera->getRadius());
            break;
        case 'n':
            renderer->advance(10);
            break;
    }
    // Redraw the scene
    glutPostRedisplay();
}

void KeyboardHandlers::specialKeyHandler(int key, int x, int y) {
    if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
        // If the shift key is pressed, do translation instead of rotation
        switch (key) {
            case GLUT_KEY_LEFT:
                *objectPosition = *objectPosition + (-0.02 * camera->getRadius() * camera->right());
                break;
            case GLUT_KEY_UP:
                *objectPosition = *objectPosition + (0.02 * camera->getRadius() * camera->up());
                break;
            case GLUT_KEY_RIGHT:
                *objectPosition = *objectPosition + (0.02 * camera->getRadius() * camera->right());
                break;
            case GLUT_KEY_DOWN:
                *objectPosition = *objectPosition + (-0.02 * camera->getRadius() * camera->up());
                break;
        }
    } else {
        // If the shift key isn't pressed, do rotations
        switch (key) {
            case GLUT_KEY_LEFT:
                camera->rotateH(5);
                break;
            case GLUT_KEY_UP:
                camera->rotateV(5);
                break;
            case GLUT_KEY_RIGHT:
                camera->rotateH(-5);
                break;
            case GLUT_KEY_DOWN:
                camera->rotateV(-5);
                break;
        }
    }
    // Redraw the scene
    glutPostRedisplay();
}

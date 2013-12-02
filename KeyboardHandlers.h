#ifndef KEYBOARD_HANDLERS_H
#define KEYBOARD_HANDLERS_H

#include "Camera.h"
#include "Renderer.h"
#include "algebra3.h"

class KeyboardHandlers {
    public:
        KeyboardHandlers(Camera* c, vec3* pos, Renderer* r);
        void basicKeyHandler(unsigned char key, int x, int y);
        void specialKeyHandler(int key, int x, int y);
    private:
        Camera* camera;
        vec3* objectPosition;
        Renderer* renderer;
};

#endif

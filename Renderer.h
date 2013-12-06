#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <vector>
#include "algebra3.h"
#include "Camera.h"
#include "Particle.h"
#include "Surface.h"
class Simulator;
class Renderer {
    public:
        Surface surface;
        Simulator* simulator;
        Renderer(Camera* c, vec3* pos, Simulator* sim);
        void render();
        void advance(int steps);
        void reshape(int w, int h);

    private:
        Camera* camera;
        vec3* objectOffset;
};

#endif

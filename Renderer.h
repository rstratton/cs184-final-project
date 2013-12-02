#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <vector>
#include "algebra3.h"
#include "Camera.h"
#include "Particle.h"

class Renderer {
    public:
        std::vector<Particle>* particles;
        Renderer(Camera* c, vec3* pos, std::vector<Particle>* particles);
        void render();
        void reshape(int w, int h);

    private:
        Camera* camera;
        vec3* objectOffset;
};

#endif

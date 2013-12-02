#ifndef __final__ParticleInspector__
#define __final__ParticleInspector__

#include <float.h>
#include <vector>
#include <iostream>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <GLUT/glut.h>
#include "Camera.h"
#include "Renderer.h"
#include "KeyboardHandlers.h"
#include "Fluid.h"
#include "Particle.h"
#include "algebra3.h"
#include "StaticObject.h"
#include "Simulator.h"

class ParticleInspector {
    public:
        ParticleInspector(Simulator* s);
        void run(int argc, char** argv);

    private:
        Simulator *sim;
};

#endif

#include <float.h>
#include <vector>
#include <iostream>
#include <string>
#include <math.h>
#include <stdlib.h>

#if defined(OSX) || defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

#include "Camera.h"
#include "Renderer.h"
#include "KeyboardHandlers.h"
#include "Fluid.h"
#include "Particle.h"
#include "algebra3.h"
#include "StaticObject.h"
#include "Simulator.h"
#include "ParticleInspector.h"
using namespace std;

// This namespace prevents this variables from being accessible anywhere but in
// this file
namespace {
    // The camera eye position
    Camera* camera; // = new Camera(0, 0, 10);

    // The offset of objects in the scene from the origin
    vec3* objectOffset; // = new Point();

    // The scene renderer
    Renderer* renderer; // = new Renderer(camera, objectOffset);

    // The keyboard callback handlers
    KeyboardHandlers* handlers; // = new KeyboardHandlers(camera, objectOffset, renderer);
}

void displayCallback() {
    renderer->render();
}

void reshapeCallback(int w, int h) {
    renderer->reshape(w, h);
}

void basicKeyCallback(unsigned char key, int x, int y) {
    handlers->basicKeyHandler(key, x, y);
}

void specialKeyCallback(int key, int x, int y) {
    handlers->specialKeyHandler(key, x, y);
}

ParticleInspector::ParticleInspector(Simulator*s) {
    this->sim = s;
}

void ParticleInspector::run(int argc, char** argv) {
    //Simulator* sim = new Simulator(SceneProperties(), vector<StaticObject>(), vector<FluidVolume>());
    //vector<Particle>* particles = new vector<Particle>;
    //particles = new vector<Particle>;
    //particles->push_back(Particle(vec3(0, 0, 0), FluidProperties(), sim));
    //particles->push_back(Particle(vec3(1, 0, 0), FluidProperties(), sim));
    //particles->push_back(Particle(vec3(0, 1, 0), FluidProperties(), sim));
    //particles->push_back(Particle(vec3(0, 0, 1), FluidProperties(), sim));
    //particles->push_back(Particle(vec3(1, 1, 0), FluidProperties(), sim));
    //particles->push_back(Particle(vec3(1, 0, 1), FluidProperties(), sim));
    //particles->push_back(Particle(vec3(0, 1, 1), FluidProperties(), sim));
    //particles->push_back(Particle(vec3(1, 1, 1), FluidProperties(), sim));

    vec3 sum;
    float minX = FLT_MAX;
    float minY = FLT_MAX;
    float minZ = FLT_MAX;
    float maxX = FLT_MIN;
    float maxY = FLT_MIN;
    float maxZ = FLT_MIN;
    for (int i = 0; i < sim->allParticles.size(); ++i) {
        vec3 pos = sim->allParticles[i].position;
        minX = fmin(minX, pos[VX]);
        minY = fmin(minY, pos[VY]);
        minZ = fmin(minZ, pos[VZ]);
        maxX = fmax(maxX, pos[VX]);
        maxY = fmax(maxY, pos[VY]);
        maxZ = fmax(maxZ, pos[VZ]);
        sum = sum + pos;
    }
    vec3 centroid = sum * (1.0 / (sim->allParticles.size()));

    minX -= centroid[VX];
    maxX -= centroid[VX];
    minY -= centroid[VY];
    maxY -= centroid[VY];
    minZ -= centroid[VZ];
    maxZ -= centroid[VZ];
    float max_distance = FLT_MIN;
    max_distance = fmax(fabs(minX), fabs(maxX));
    max_distance = fmax(max_distance, fabs(minZ));
    max_distance = fmax(max_distance, fabs(maxZ));
    float radius =  2 * (1 / tan((35.0 / 2) * (PI / 180))) * max_distance + maxY;

    camera = new Camera(0, 0, radius);
    objectOffset = new vec3(-centroid[VX], -centroid[VY], -centroid[VZ]);
    renderer = new Renderer(camera, objectOffset, sim);
    handlers = new KeyboardHandlers(camera, objectOffset, renderer);


    // Initialize GLUT and create window
    glutInit(&argc, argv);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(700, 700);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Movement Test");

    // Set callbacks
    glutDisplayFunc(displayCallback);
    glutReshapeFunc(reshapeCallback);
    glutKeyboardFunc(basicKeyCallback);
    glutSpecialFunc(specialKeyCallback);
    glutMainLoop();
}

#include <string>
#include <vector>
#include <GLUT/glut.h>
#include "Renderer.h"
#include "algebra3.h"
#include "Camera.h"
#include "Particle.h"
#include "ParticleInspector.h"
using namespace std;

Renderer::Renderer(Camera* c, vec3* pos, vector<Particle>* particles) {
    camera = c;
    objectOffset = pos;
    this->particles = particles;
}

void drawParticle(Particle& particle) {
    glVertex3f(particle.position[VX], particle.position[VY], particle.position[VZ]);
}

void Renderer::render() {
    // Clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reset transformation to identity
    glLoadIdentity();

    // Update the camera position
    camera->update();

    // Get the (x, y, z) offset for objects in the scene and set the
    // transformation state to the correct amount
    float x = (*objectOffset)[VX];
    float y = (*objectOffset)[VY];
    float z = (*objectOffset)[VZ];
    glTranslatef(x, y, z);

    glBegin(GL_POINTS);
    for (int i = 0; i < particles->size(); ++i) {
        drawParticle((*particles)[i]);
    }
    glEnd();

    // Set the drawn scene to the display buffer
    glutSwapBuffers();
    glFlush();
}

void Renderer::reshape(int w, int h) {
    // Don't allow height be be 0, to avoid divide-by-zero
    if (h == 0) h = 1;
    // Calculate the width-to-height ratio
    float ratio = float(w) / h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    // Use a FOV of 45 degrees.  Set the near/far clipping plane to 1/1000
    gluPerspective(45, ratio, 0.1, 1000);
    glMatrixMode(GL_MODELVIEW);
}

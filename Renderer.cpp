#include <string>
#include <vector>

#ifdef OSX
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

#include "Renderer.h"
#include "algebra3.h"
#include "Camera.h"
#include "Particle.h"
#include "ParticleInspector.h"
#include "Simulator.h"
using namespace std;

Renderer::Renderer(Camera* c, vec3* pos, Simulator* sim) {
    camera = c;
    objectOffset = pos;
    this->simulator = sim;
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
  
    glColor3f(0, 1, 0);
    glBegin(GL_POINTS);
    for (int i = 0; i < simulator->allParticles.size(); ++i) {
        drawParticle(simulator->allParticles[i]);
    }
    glEnd();
  
    glColor3f(0, 0, 1);
    for(int i =0; i < simulator->objects.size(); i++) {
      glBegin(GL_LINE_LOOP);
      for(int j = 0; j < simulator->objects[i]->faces.size(); j++) {
        for(int k = 0; k < ((Triangle*)simulator->objects[i]->faces[j])->vertices.size(); k++) {
          glVertex3f(((Triangle*)simulator->objects[i]->faces[j])->vertices[k][0], ((Triangle*)simulator->objects[i]->faces[j])->vertices[k][1],((Triangle*)simulator->objects[i]->faces[j])->vertices[k][2]);
        }
      }
      glEnd();
    }

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

void Renderer::advance(int n) {
  for(int i = 0; i < n; i++)
    simulator->advanceTimeStep();
  glutPostRedisplay();
}

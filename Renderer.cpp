#include <string>
#include <vector>
#include <stdlib.h>
#if defined(OSX) || defined(__APPLE__)
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
  glBegin(GL_POINTS);

    glVertex3f(particle.position[VX], particle.position[VY], particle.position[VZ]);
  glEnd();

//    glBegin(GL_LINE_LOOP);
//  glColor3f(0,0,1);
//
//    glVertex3f(particle.position[VX], particle.position[VY], particle.position[VZ]);
//  glColor3f(0,1,0);
//
//    glVertex3f(particle.position[VX] + particle.acceleration[VX], particle.position[VY] + particle.acceleration[VY], particle.position[VZ] + particle.acceleration[VZ]);
//    glEnd();
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
    for (int i = 0; i < simulator->allParticles.size(); ++i) {
        drawParticle(simulator->allParticles[i]);
    }
  
    glColor3f(0, 0, 1);
    for(int i =0; i < simulator->objects.size(); i++) {
      //glBegin(GL_LINE_LOOP);
      glBegin(GL_LINES);
      for(int j = 0; j < simulator->objects[i]->faces.size(); j++) {
        for(int k = 0; k < ((Triangle*)simulator->objects[i]->faces[j])->vertices.size(); k++) {
          glVertex3f(((Triangle*)simulator->objects[i]->faces[j])->vertices[k][0], ((Triangle*)simulator->objects[i]->faces[j])->vertices[k][1],((Triangle*)simulator->objects[i]->faces[j])->vertices[k][2]);
          glVertex3f(((Triangle*)simulator->objects[i]->faces[j])->vertices[(k+1)%3][0], ((Triangle*)simulator->objects[i]->faces[j])->vertices[(k+1)%3][1],((Triangle*)simulator->objects[i]->faces[j])->vertices[(k+1)%3][2]);
        }
      }
      glEnd();
    }
  glColor3f(0, 0, 1);
    glBegin(GL_LINE_LOOP);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1000);
    glEnd();
  glColor3f(1, 0, 0);

    glBegin(GL_LINE_LOOP);
    glVertex3f(0, 0, 0);
    glVertex3f(1000, 0, 0);
    glEnd();
  glColor3f(0, 1, 0);
glBegin(GL_LINE_LOOP);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 1000, 0);
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
    gluPerspective(35, ratio, 0.1, 100000);
    glMatrixMode(GL_MODELVIEW);
}

void Renderer::advance(int n) {
  for(int i = 0; i < n; i++)
    simulator->advanceTimeStep();
  glutPostRedisplay();
}

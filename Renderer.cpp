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
#include "Surface.h"
using namespace std;

Renderer::Renderer(Camera* c, vec3* pos, Simulator* sim) : surface(sim) {
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

void drawLatticePoint(Surface& surface, int i, int j, int k) {
    LatticePoint lp = surface.latticePoints[i][j][k];
    if (lp.included) {
        glVertex3f(lp.position[VX], lp.position[VY], lp.position[VZ]);
    }
}

void drawLatticePoints(Surface& surface) {
    glBegin(GL_POINTS);
    for (int i = 0; i < surface.xSamples; ++i) {
        for (int j = 0; j < surface.ySamples; ++j) {
            for (int k = 0; k < surface.zSamples; ++k) {
                drawLatticePoint(surface, i, j, k);
            }
        }
    }
    glEnd();
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
  

    if (properties.showParticles) {
        glColor3f(0, 1, 0);
        for (int i = 0; i < simulator->allParticles.size(); ++i) {
            drawParticle(simulator->allParticles[i]);
        }
    }

    //if (properties.showLattice || properties.showMesh) {
    //    surface.resample();
    //}

    if (properties.showLattice) {
        // Draw lattice points used in surface reconstruction
        glColor3f(1, 0, 0);
        drawLatticePoints(surface);
    }

    if (properties.showMesh) {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);

        GLfloat mat_specular[] = { 0.9, 0.9, 0.9, 1.0 };
        GLfloat mat_shininess[] = { 20.0 };
        GLfloat mat_diffuse[] = { 0.9, 0.9, 0.9, 1.0 };
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

        glEnable(GL_LIGHT0);
        GLfloat l0_position[] = { -10.0, -10.0, -2, 0.0 };
        GLfloat l0_diffuse[] = { 0.5, 0.5, 0.9, 1.0 };
        GLfloat l0_specular[] = { 0.5, 0.5, 0.9, 1.0 };
        GLfloat l0_shininess[] = { 20.0 };
        glLightfv(GL_LIGHT0, GL_DIFFUSE, l0_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, l0_specular);
        glLightfv(GL_LIGHT0, GL_SHININESS, l0_shininess);
        glLightfv(GL_LIGHT0, GL_POSITION, l0_position);

        glEnable(GL_LIGHT1);
        GLfloat l1_position[] = { 10.0, 0.0, 0.0, 0.0 };
        GLfloat l1_diffuse[] = { 0.5, 0.9, 0.5, 1.0 };
        GLfloat l1_specular[] = { 0.5, 0.9, 0.5, 1.0 };
        GLfloat l1_shininess[] = { 20.0 };
        glLightfv(GL_LIGHT1, GL_DIFFUSE, l1_diffuse);
        glLightfv(GL_LIGHT1, GL_SPECULAR, l1_specular);
        glLightfv(GL_LIGHT1, GL_SHININESS, l1_shininess);
        glLightfv(GL_LIGHT1, GL_POSITION, l1_position);

        glEnable(GL_LIGHT2);
        GLfloat l2_position[] = { 0.0, 0.0, 10.0, 0.0 };
        GLfloat l2_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
        GLfloat l2_specular[] = { 0.5, 0.5, 0.5, 1.0 };
        GLfloat l2_shininess[] = { 20.0 };
        glLightfv(GL_LIGHT2, GL_DIFFUSE, l2_diffuse);
        glLightfv(GL_LIGHT2, GL_SPECULAR, l2_specular);
        glLightfv(GL_LIGHT2, GL_SHININESS, l2_shininess);
        glLightfv(GL_LIGHT2, GL_POSITION, l2_position);

        vector<MeshTriangle>* triangles = surface.getMesh();
        glColor3f(1, 1, 0);
        //glBegin(GL_LINES);
        glBegin(GL_TRIANGLES);
        for (int i = 0; i < triangles->size(); ++i) {
            //(*triangles)[i].drawOutline();
            (*triangles)[i].drawFace();
        }
        glEnd();
        glDisable(GL_LIGHTING);
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

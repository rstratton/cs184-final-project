//
//  OpenGLRenderer.cpp
//  final
//
//  Created by Kavan Sikand on 12/1/13.
//  Copyright (c) 2013 Kavan Sikand. All rights reserved.
//

#include "OpenGLRenderer.h"
#include "Simulator.h"
#include <thread>
void oglRenderer::myDisplay (){
  vector<Particle>*particles = &(currInstance->particleHistory[currInstance->displayFrame]);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// clear the color buffer
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, 600, 0, 400, 200, -300);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glColor3f(1, 0, 0);
  glBegin(GL_POINTS);
  for(int i = 0; i < particles->size(); i++) {
    glVertex3f(particles->at(i).position[0], particles->at(i).position[1], particles->at(i).position[2]);
  }
  glEnd();
  
  Simulator* sim = currInstance->sim;
  glColor3f(0, 0, 1);
  for(int i =0; i < sim->objects.size(); i++) {
    glBegin(GL_LINE_LOOP);
    for(int j = 0; j < sim->objects[i]->faces.size(); j++) {
      for(int k = 0; k < ((Triangle*)sim->objects[i]->faces[j])->vertices.size(); k++) {
        glVertex3f(((Triangle*)sim->objects[i]->faces[j])->vertices[k][0], ((Triangle*)sim->objects[i]->faces[j])->vertices[k][1],((Triangle*)sim->objects[i]->faces[j])->vertices[k][2]);
      }
    }
    glEnd();
  }
  glFlush();
  glutSwapBuffers();					// swap buffers (we earlier set double buffer)
}

void oglRenderer::updateParticles(int i ) {
  particleHistory[i] = sim->allParticles;
}

void keyPressed(unsigned char key, int x, int y) {
  if(key == ' ') {
    //case for spacebar
    exit(0);
  }
}
void specialKeyPressed(int key, int x, int y) {
  if(key==GLUT_KEY_LEFT) {
    currInstance->displayFrame = max(0, currInstance->displayFrame-10);
  }else if(key==GLUT_KEY_RIGHT) {
    currInstance->displayFrame = currInstance->displayFrame+10;
  }
  glutPostRedisplay();
}
void oglRenderer::loop() {
  glutMainLoop();
}

void oglRenderer::initialize(int argc, const char * argv[]) {
  //This initializes glut
  glutInit(&argc, (char**) argv);
  //This tells glut to use a double-buffered window with red, green, and blue channels and a depth buffer
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  
  
  //The size and position of the window
  glutInitWindowSize(600,400);
  glutInitWindowPosition(0,0);
  
  glutCreateWindow("Bezier Surfaces");
  currInstance = this;
  glutDisplayFunc(Renderer::myDisplay);
  glutKeyboardFunc(keyPressed);
  glutSpecialFunc(specialKeyPressed);
  glClearColor (0.0, 0.0, 0.0, 0.0);
}



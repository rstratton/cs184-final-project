//
//  OpenGLRenderer.h
//  final
//
//  Created by Kavan Sikand on 12/1/13.
//  Copyright (c) 2013 Kavan Sikand. All rights reserved.
//

#ifndef __final__OpenGLRenderer__
#define __final__OpenGLRenderer__

#include <iostream>
#include <GLUT/GLUT.h>
#include <map>
#include "Particle.h"

class Simulator;
class Renderer;
static oglRenderer* currInstance;
class oglRenderer {
  Simulator* sim;
  //for debugging, to allow stepping through simulation
  std::map<int,vector<Particle> >  particleHistory;
  public:
    int displayFrame = 0;
    int numTimesteps;
    Renderer(Simulator*s, int num) : sim(s), numTimesteps(num) {
     particleHistory = std::map<int,vector<Particle> >();
    };
    void initialize(int argc, const char * argv[]);
    static void myDisplay();
    void loop();
    void updateParticles(int i);
};


#endif /* defined(__final__OpenGLRenderer__) */

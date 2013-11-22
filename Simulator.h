//
//  Simulator.h
//  final
//
//  Created by Kavan Sikand on 11/21/13.
//  Copyright (c) 2013 Kavan Sikand. All rights reserved.
//

#ifndef __final__Simulator__
#define __final__Simulator__
#include <iostream>
#include "algebra3.h"
#include "Particle.h"
#include <math.h>
#include <vector>
#include <list>
#include <map>
#include <utility>

#define PI 3.14159
using namespace std;
static float smoothing = 200;
static const int gridCells = 10;

static float kernelFunction(vec3 difference) {
  if(difference.length() > 2*smoothing) {
    return 0;
  }
  //using one from the paper
  return 1/(pow(PI,1.5)*pow(smoothing,3.))*exp(pow(difference.length(),2.)/pow(smoothing,2.));
}

class Simulator {
  vector<Particle> allParticles;
  //according to one of the research papers, we should keep a 3x3 grid of which cells particles are in.
  //This will reduce particle-to-particle interaction times to O(n) instead of O(n^2)
  
  //using maps is memory intensive but allows fast lookup. The other option is a 3x3 array. We can look into what works better.
  map<tuple<int, int, int>,list<Particle*> > particleGridCurrent; //list because of fast insert and deletes
  map<tuple<int, int, int>,list<Particle*> > particleGridNext; //because of fast insert and deletes
  
  //here's the 3x3 array method
  list<Particle*> particleGrid[gridCells][gridCells][gridCells];
  list<Particle*> nextParticleGrid[gridCells][gridCells][gridCells];


  void initialize();
  
  vector<Particle*> getNeighborsForParticle(Particle p);
  
};

#endif /* defined(__final__Simulator__) */

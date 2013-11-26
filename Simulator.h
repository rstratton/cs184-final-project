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
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 400
#define SCENE_DEPTH 200
#define gridCells 10

using namespace std;

const float smoothing = 3;

class Simulator {
  float timestep;
  int numTimesteps;
  vector<Particle> allParticles;
  //talked to the professor, seems like a list of int indices is the best method.
  list<unsigned int> particleGrid[gridCells][gridCells][gridCells];
  list<unsigned int> nextParticleGrid[gridCells][gridCells][gridCells];


  
  public:
    void initialize();
    void advanceTimeStep();
    void runSimulation();
    static float kernelFunction(vec3 difference) {
      if(difference.length() > 2*smoothing) {
        return 0;
      }
      //using one from the paper
      float term =1/(pow(PI,1.5)*pow(smoothing,3.));
      float e = exp(pow(difference.length(),2.)/pow(smoothing,2.));
      
      return term * e;
    }
    void addParticle(Particle p);

  private:
    vector<Particle*> getNeighborsForParticle(unsigned int i); //return a list of indices
    void printParticleGrid();
  
};

#endif /* defined(__final__Simulator__) */

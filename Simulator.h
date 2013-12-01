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

const float smoothing = 10;

class Simulator {
  float timestep;
  int numTimesteps;
  int cutoff;
  vector<Particle> allParticles;
  //talked to the professor, seems like a list of int indices is the best method.
  vector<vector<vector<list<unsigned int> > > > particleGrid;
  vector<vector<vector<list<unsigned int> > > > nextParticleGrid;


  
public:
    vec3 worldSize;
    int numGridCells;
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
    void addParticle(vec3 pos, FluidProperties fp);
    Simulator(vec3 ws) : worldSize(ws) {};

  private:
    vector<Particle*> getNeighborsForParticle(unsigned int i); //return a list of indices
    void printParticleGrid();
  
};

#endif /* defined(__final__Simulator__) */

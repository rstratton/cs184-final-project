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
#include "SceneParser.h"
#define PI 3.14159

using namespace std;


class Simulator {
  
  int cutoff;

  //talked to the professor, seems like a list of int indices is the best method.
  vector<vector<vector<list<unsigned int> > > > particleGrid;
  vector<vector<vector<list<unsigned int> > > > nextParticleGrid;


  
  public:
    SceneProperties properties;
    int numGridCells;
    void initialize();
    void addParticle(vec3 pos, FluidProperties fp);
    void advanceTimeStep();
    void printParticleGrid();
    Simulator(SceneProperties p) : properties(p) {};

  protected:
    friend struct Particle;
    float kernelFunction(vec3 difference);
    vector<Particle> allParticles;

  private:
    vector<Particle*> getNeighborsForParticle(unsigned int i); //return a list of indices
  
};

#endif /* defined(__final__Simulator__) */

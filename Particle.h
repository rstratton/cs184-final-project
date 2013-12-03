//
//  Particle.h
//  final
//
//  Created by Kavan Sikand on 11/21/13.
//  Copyright (c) 2013 Kavan Sikand. All rights reserved.
//

#ifndef __final__Particle__
#define __final__Particle__

#include <iostream>
#include <vector>
#include "algebra3.h"
#include "Color.h"
#include "Fluid.h"
class Simulator;
using namespace std;

//for using the grid data structure.
struct GridPosition {
  int x, y, z;
  GridPosition(int a, int b, int c) :x(a), y(b), z(c){};
  GridPosition() :x(0), y(0), z(0){};
};




static vec3 gravity = vec3(0,-9.8,0); //in m/s/s

struct Particle {
  FluidProperties fp;
  vec3 position;
  vec3 velocity;
  vec3 force;
  vec3 acceleration;
  vector <Particle*> neighbors;
  float pressure;
  float density;
  GridPosition gridPosition;
  Simulator* sim;
  Particle (vec3 initialPos, FluidProperties fluid, Simulator* s);
 
  float calculateDensity();
  void calculateForces();
  void advanceTimeStep(float timestep, int numGridCells);
};

#endif /* defined(__final__Particle__) */

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
using namespace std;

struct FluidProperties {
  float mass;
  float viscosity;
  float pressureConstant;
  Color color;
  FluidProperties(float m, float v, float p, Color c) : mass(m), viscosity(v), pressureConstant(p), color(c) {};
};

struct GridPosition {
  int x, y, z;
  GridPosition(int a, int b, int c) :x(a), y(b), z(c){};
  GridPosition() :x(0), y(0), z(0){};
};

static vec3 gravity = vec3(0,-4.9,0);

struct Particle {
  FluidProperties fp;
  vec3 position;
  vec3 velocity;
  vec3 force;
  vec3 acceleration;
  GridPosition gridPosition;
  Particle (vec3 initialPos, FluidProperties fluid, int numCells);
 
  float calculateDensity(vector <Particle*> neighbors);
  void calculateForces(vector <Particle*> neighbors);
  void advanceTimeStep(int numCells, float timestep);
};

#endif /* defined(__final__Particle__) */

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
};

struct GridPosition {
  int x, y, z;
};

static vec3 gravity = vec3(0,-4.9,0);

struct Particle {
  FluidProperties fp;
  vec3 position;
  vec3 velocity;
  vec3 force;
  
  GridPosition gridPosition;
  vec3 getAcceleration() {
    return force / fp.mass;
  }
  float calculateDensity(vector <Particle*> neighbors);
  void calculateForces();
  
};

#endif /* defined(__final__Particle__) */

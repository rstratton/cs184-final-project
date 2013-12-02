//
//  FluidVolume.h
//  final
//
//  Created by Kavan Sikand on 11/28/13.
//  Copyright (c) 2013 Kavan Sikand. All rights reserved.
//

#ifndef __final__FluidVolume__
#define __final__FluidVolume__

#include <iostream>
#include "Color.h"
#include "algebra3.h"
#include <vector>
using namespace std;
class Simulator;

//defines the properties of a fluid
struct FluidProperties {
  float mass;
  float viscosity;
  float pressureConstant;
  float elasticity; //for solid collision
  float restDensity=1;
  Color color;
  
  FluidProperties(float m, float v, float p, float e, Color c) : mass(m), viscosity(v), pressureConstant(p), elasticity(e), color(c) {};
  FluidProperties() : color(0,0,0) {};
};


//this class will be where we store the initial volume in which the fluid exists at the start. It will creating and evenly distributing particles within this volume
class FluidVolume {
private:
  string volumeType;
  FluidProperties fluid;
  vector<vec3> points;
public:
  void populateFluid(Simulator* sim);
  FluidVolume(FluidProperties fp, string v, vector<string> args); //the args will change depending on the type of volume (sphere or rect)
};

#endif /* defined(__final__FluidVolume__) */


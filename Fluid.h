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
#include <vector>
using namespace std;
class Simulator;

//defines the properties of a fluid
struct FluidProperties {
  float mass;
  float viscosity;
  float pressureConstant;
  Color color;
  FluidProperties(float m, float v, float p, Color c) : mass(m), viscosity(v), pressureConstant(p), color(c) {};
  FluidProperties() : color(0,0,0) {};
};


//this class will be where we store the initial volume in which the fluid exists at the start. It will creating and evenly distributing particles within this volume
class FluidVolume {
private:
  string volumeType;
  FluidProperties fluid;
public:
  void populateFluid(Simulator* sim);
  FluidVolume(FluidProperties fp, string v, vector<string> args); //the args will change depending on the type of volume (sphere or rect)
};

#endif /* defined(__final__FluidVolume__) */


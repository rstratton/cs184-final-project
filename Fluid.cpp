//
//  FluidVolume.cpp
//  final
//
//  Created by Kavan Sikand on 11/28/13.
//  Copyright (c) 2013 Kavan Sikand. All rights reserved.
//

#include "Fluid.h"
#include "Simulator.h"
#define PI 3.14159
void FluidVolume::populateFluid(Simulator* sim) {
  //now actually populate the fluid
  //for now just add a particle
  for(int i = 0; i < points.size(); i++) {
    sim->addParticle(points[i], fluid);
  }
}

FluidVolume::FluidVolume(FluidProperties fp, string v, vector<string> args) : volumeType(v), fluid(fp) {
  //process the arguments or whatever
  points = vector<vec3> ();
  if(volumeType == "sphere") {
    vec3 center = vec3(atof(args[0].c_str()), atof(args[1].c_str()), atof(args[2].c_str()));
    float rad = atof(args[3].c_str());
    int numParticles = atof(args[4].c_str());
    
    //to evenly distribute points around the sphere, we will evenly distribute points around the surface of spheres of radii evenly spaced up to the target radius
    //this algorithm will get the radius and spacing even, but just uses "in the ballpark of numParticles"
    //i.e. its not exactly the numParticles, but its close-ish
    float divisions = floor(pow(numParticles,1./3.));
    for(float radius = rad; radius > 0; radius -= rad / divisions) {
      for(float phi = 2*PI; phi > 0; phi -= (2*PI) / divisions) {
        for(float theta = 2*PI; theta > 0; theta -= (2*PI)/ divisions) {
          float x = center[0] + rad*cos(phi);
          float y = center[1] + rad*sin(theta);
          float z = center[2] + rad*sin(phi);
          points.push_back(vec3(x,y,z));
        }
      }
    }
  }
}
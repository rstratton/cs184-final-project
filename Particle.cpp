//
//  Particle.cpp
//  final
//
//  Created by Kavan Sikand on 11/21/13.
//  Copyright (c) 2013 Kavan Sikand. All rights reserved.
//

#include "Particle.h"
#include "Simulator.h"


float Particle::calculateDensity(vector<Particle*> neighbors){ //todo: USE LINKED LIST INSTEAD OF VECTOR
  float sum = 0;
  for(int j = 0; j < neighbors.size(); j++) {
    sum += neighbors[j].fp.mass*kernelFunction(position - neighbors[j].position);
  }
  return sum;
}

void Particle::calculateForces()  { //list of neighbors
  vector<Particle> particles = vector<Particle>();
  float density = calculateDensity(particles);
  force = gravity*density;
}
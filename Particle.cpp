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
    sum += neighbors[j]->fp.mass*Simulator::kernelFunction(position - neighbors[j]->position);
  }
  return sum;
}

void Particle::calculateForces(vector <Particle*> neighbors)  { //list of neighbors
  float density = calculateDensity(neighbors);
  force = gravity*density; //need ot add other terms
  acceleration = force/density; //is this right? not sure how to get acceleration once we have the force
}

void Particle::advanceTimeStep(float timestep, int numGridCells) {
  velocity = velocity + acceleration*timestep;
  position += velocity;
  
  gridPosition.x = floor((position[0]/sim->worldSize[0])*numGridCells);
  gridPosition.y = floor((position[1]/sim->worldSize[1])*numGridCells);
  gridPosition.z = floor((position[2]/sim->worldSize[2])*numGridCells);
}

Particle::Particle(vec3 initialPos, FluidProperties fluid, Simulator* s) : fp(fluid), position(initialPos), sim(s) {
  gridPosition = GridPosition(floor((position[0]/sim->worldSize[0])*sim->numGridCells), floor((position[1]/sim->worldSize[1])*sim->numGridCells), floor((position[2]/sim->worldSize[2])*sim->numGridCells));
  
}
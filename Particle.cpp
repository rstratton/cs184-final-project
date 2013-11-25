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

void Particle::advanceTimeStep(int numCells, float timestep) {
  velocity = velocity + acceleration*timestep;
  position += velocity;
  
  gridPosition.x = floor((position[0]/WINDOW_WIDTH)*numCells);
  gridPosition.y = floor((position[1]/WINDOW_HEIGHT)*numCells);
  gridPosition.z = floor((position[2]/SCENE_DEPTH)*numCells);
}

Particle::Particle(vec3 initialPos, FluidProperties fluid, int numCells) : position(initialPos), fp(fluid) {
  gridPosition = GridPosition(floor((position[0]/WINDOW_WIDTH)*numCells), floor((position[1]/WINDOW_HEIGHT)*numCells), floor((position[2]/SCENE_DEPTH)*numCells));
  
}
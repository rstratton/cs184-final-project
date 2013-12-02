//
//  Particle.cpp
//  final
//
//  Created by Kavan Sikand on 11/21/13.
//  Copyright (c) 2013 Kavan Sikand. All rights reserved.
//

#include "Particle.h"
#include "Simulator.h"


float Particle::calculateDensity(){ //todo: USE LINKED LIST INSTEAD OF VECTOR
  float sum = 0;
  for(int j = 0; j < neighbors.size(); j++) {
    sum += neighbors[j]->fp.mass*sim->kernelFunction(position - neighbors[j]->position);
  }
  return sum;
}

void Particle::calculateForces()  { //list of neighbors
  float density = calculateDensity(neighbors);
  vec3 pressureForce;
  vec3 viscosityForce;
  for(int i = 0; i < neighbors.length; i++) {
    pressureForce -= (pressure + neighbors[i]->pressure)/2 * fp.mass/neighbors[i]->calculateDensity() * sim->pressureGradient(position - neighbors[i]->position);
    viscosityForce += fp.viscosity * (neighbors[i]->velocity - velocity) * fp.mass/neighbors[i]->calculateDensity() * viscosityGradient(position - neighbors[i]->position);
  }
  force = pressureForce + viscosityForce + gravity*density; 
  acceleration = force/density;
}

void Particle::advanceTimeStep(float timestep, int numGridCells) {
  velocity += acceleration*timestep;
  position += velocity*timestep;
  
  gridPosition.x = floor((position[0]/sim->properties.worldSize[0])*numGridCells);
  gridPosition.y = floor((position[1]/sim->properties.worldSize[1])*numGridCells);
  gridPosition.z = floor((position[2]/sim->properties.worldSize[2])*numGridCells);
}

Particle::Particle(vec3 initialPos, FluidProperties fluid, Simulator* s) : fp(fluid), position(initialPos), sim(s) {
  gridPosition = GridPosition();
  gridPosition.x = floor((position[0]/sim->properties.worldSize[0])*sim->numGridCells);
  gridPosition.y = floor((position[1]/sim->properties.worldSize[1])*sim->numGridCells);
  gridPosition.z = floor((position[2]/sim->properties.worldSize[2])*sim->numGridCells);
  
}

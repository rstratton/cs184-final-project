//
//  Particle.cpp
//  final
//
//  Created by Kavan Sikand on 11/21/13.
//  Copyright (c) 2013 Kavan Sikand. All rights reserved.
//

#include "Particle.h"
#include "Simulator.h"

void Particle::advanceTimeStep(float timestep, vec3 numGridCells) {
  velocity += acceleration*timestep;
  position += velocity*timestep;
  
  gridPosition.x = floor((position[0]/sim->properties.worldSize[0])*numGridCells[0]);
  gridPosition.y = floor((position[1]/sim->properties.worldSize[1])*numGridCells[1]);
  gridPosition.z = floor((position[2]/sim->properties.worldSize[2])*numGridCells[2]);
}

Particle::Particle(vec3 initialPos, FluidProperties* fluid, Simulator* s) : fp(fluid), position(initialPos), sim(s) {
  gridPosition = GridPosition();
  gridPosition.x = floor((position[0]/sim->properties.worldSize[0])*sim->numGridCells[0]);
  gridPosition.y = floor((position[1]/sim->properties.worldSize[1])*sim->numGridCells[1]);
  gridPosition.z = floor((position[2]/sim->properties.worldSize[2])*sim->numGridCells[2]);
  
}

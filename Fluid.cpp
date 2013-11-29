//
//  FluidVolume.cpp
//  final
//
//  Created by Kavan Sikand on 11/28/13.
//  Copyright (c) 2013 Kavan Sikand. All rights reserved.
//

#include "Fluid.h"
#include "Simulator.h"
void FluidVolume::populateFluid(Simulator* sim) {
  //now actually populate the fluid
  //for now just add a particle
  sim->addParticle(vec3(0,400,0), fluid);
}

FluidVolume::FluidVolume(FluidProperties fp, string v, vector<string> args) : volumeType(v), fluid(fp) {
  //process the arguments or whatever
}
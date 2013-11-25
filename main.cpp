//
//  main.cpp
//  final
//
//  Created by Kavan Sikand on 11/19/13.
//  Copyright (c) 2013 Kavan Sikand. All rights reserved.
//

#include <iostream>
#include "Simulator.h"
#include "Particle.h"
#include "Color.h"
#define gridCells 10
int main(int argc, const char * argv[])
{
  Simulator s1 = Simulator();
  s1.initialize();
  FluidProperties fp = FluidProperties(1, 3, 3, Color(.2, .2, .8));
  Particle* p = new Particle(vec3(0,300,0), fp, gridCells);
  s1.addParticle(p);
  s1.runSimulation();
//  delete p;

  std::cout << "Hello, World!\n";
    return 0;
}


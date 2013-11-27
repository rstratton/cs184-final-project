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
#include "SceneParser.h"
#include "Color.h"
int main(int argc, const char * argv[])
{
  if(argc <= 1) {
    printf("not enough arguments \n");
    exit(1);
  }
  SceneParser p = SceneParser(argv[1]); //parses on construction
  
  Simulator s1 = Simulator(p.properties);
  s1.initialize();
  FluidProperties fp = FluidProperties(1, 3, 3, Color(.2, .2, .8));
  s1.addParticle(vec3(0,300,0), fp);
  int numTimesteps = 100;
  for(int i = 0; i < numTimesteps; i++) {
    s1.advanceTimeStep();
    s1.printParticleGrid();
  }

  std::cout << "Hello, World!\n";
    return 0;
}


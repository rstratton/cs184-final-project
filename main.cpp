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
#include "unistd.h"
#include "Renderer.h"
#include "ParticleInspector.h"
#include <map>

int main(int argc, char * argv[])
{
  if(argc <= 1) {
    printf("not enough arguments \n");
    exit(1);
  }
  SceneParser *p = new SceneParser(argv[1]); //parses on construction
  int numTimesteps = atoi(argv[2]);
  Simulator s1 = Simulator(p->properties, p->objects, p->volumes);
  delete p;
  s1.initialize();
//  s1.addParticle(vec3(200,51,50), FluidProperties(1, 1, 1, 1, 1, Color(1,0,0,0)));
//  s1.addParticle(vec3(201,50,50), FluidProperties(1, 1, 1, 1, 1, Color(1,0,0,0)));
//  s1.addParticle(vec3(200,50,50), FluidProperties(1, 1, 1, 1, 1, Color(1,0,0,0)));
//  s1.addParticle(vec3(200,49,50), FluidProperties(1, 1, 1, 1, 1, Color(1,0,0,0)));
//
//  s1.addParticle(vec3(199,50,50), FluidProperties(1, 1, 1, 1, 1, Color(1,0,0,0)));

  ParticleInspector pi(&s1);
  pi.run(argc, argv);
  

//  for(int i = 0; i < numTimesteps; i++) {
//    s1.advanceTimeStep();
////    s1.printParticleGrid();
//    //now something like:
//    //reconstructor.reconstructSurface()
//    //renderer.renderSurface()
//  }

  
  
  std::cout << "Hello, World!\n";
    return 0;
}

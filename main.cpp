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
//  FluidProperties * fp = new FluidProperties(1, 1, 1, 1, .13, Color(1,0,0,0));
//  s1.addParticle(vec3(200,51,50), fp);
//  s1.addParticle(vec3(201,50,50), fp);
//  s1.addParticle(vec3(200,50,50), fp);
//  s1.addParticle(vec3(200,49,50), fp);
//  s1.addParticle(vec3(199,50,50), fp);
//
  ParticleInspector pi(&s1);
  pi.run(argc, argv);
//

//  time_t startTime = time(NULL);
//  time_t lastTime = startTime;
//  for(int i = 0; i < numTimesteps; i++) {
//    s1.advanceTimeStep();
//    time_t realTime = time(NULL);
//    if(difftime(realTime, lastTime) > 30) {
//      printf("time elapsed: %f. Iteration: %d", difftime(realTime, startTime), i);
//      lastTime = realTime;
//    }
////    s1.printParticleGrid();
//    //now something like:
//    //reconstructor.reconstructSurface()
//    //renderer.renderSurface()
//  }
//
//  printf("total Runtime: %f", difftime(time(NULL), startTime));
  
  std::cout << "Hello, World!\n";
    return 0;
}

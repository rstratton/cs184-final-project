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
#include "OpenGLRenderer.h"
#include <map>

int main(int argc, const char * argv[])
{
  if(argc <= 2) {
    printf("not enough arguments \n");
    exit(1);
  }
  SceneParser p = SceneParser(argv[1]); //parses on construction
  int numTimesteps = atoi(argv[2]);
  Simulator s1 = Simulator(p.properties, p.objects, p.volumes);
  s1.initialize();
  Renderer r1 = Renderer(&s1, numTimesteps);
  r1.initialize(argc, argv);
  
  
  for(int i = 0; i < numTimesteps; i++) {
    s1.advanceTimeStep();
    s1.printParticleGrid();

    if(i%10 ==0)
      r1.updateParticles(i);
    //now something like:
    //reconstructor.reconstructSurface()
    //renderer.renderSurface()
  }

  r1.loop();
  
  
  std::cout << "Hello, World!\n";
    return 0;
}


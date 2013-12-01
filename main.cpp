<<<<<<< HEAD
//
//  main.cpp
//  final
//
//  Created by Kavan Sikand on 11/19/13.
//  Copyright (c) 2013 Kavan Sikand. All rights reserved.
//

#include <iostream>

int main(int argc, const char * argv[])
{

  std::cout << "Hello, World!\n";
    return 0;
}

=======
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
#include "StaticObject.h"
#include "unistd.h"

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
  
  
  for(int i = 0; i < numTimesteps; i++) {
    s1.advanceTimeStep();
    s1.printParticleGrid();
    
    //now something like:
    //reconstructor.reconstructSurface()
    //renderer.renderSurface()
  }
  
  std::cout << "Hello, World!\n";
    return 0;
}

>>>>>>> 182ad5ffef771cf5f310e7370297b7b0f0846fa8

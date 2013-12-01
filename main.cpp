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
#include "Color.h"
int main(int argc, const char * argv[])
{
  Simulator s1 = Simulator(vec3(600,400,200));
  s1.initialize();
  FluidProperties fp = FluidProperties(1, 3, 3, Color(.2, .2, .8));
  s1.addParticle(vec3(0,300,0), fp);
  s1.runSimulation();

  std::cout << "Hello, World!\n";
    return 0;
}

>>>>>>> 182ad5ffef771cf5f310e7370297b7b0f0846fa8

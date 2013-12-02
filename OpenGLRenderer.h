//
//  OpenGLRenderer.h
//  final
//
//  Created by Kavan Sikand on 12/1/13.
//  Copyright (c) 2013 Kavan Sikand. All rights reserved.
//

#ifndef __final__OpenGLRenderer__
#define __final__OpenGLRenderer__

#include <iostream>
#include <GLUT/GLUT.h>
#include <thread>
class Simulator;
class Renderer;
static Renderer* currInstance;
class Renderer {
  Simulator* sim;
  public:
    Renderer(Simulator*s) : sim(s) {};
    void initialize(int argc, const char * argv[]);
    static void myDisplay();
    void loop();
};


#endif /* defined(__final__OpenGLRenderer__) */

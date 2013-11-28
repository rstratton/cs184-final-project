//
//  SceneParser.h
//  final
//
//  Created by Kavan Sikand on 11/27/13.
//  Copyright (c) 2013 Kavan Sikand. All rights reserved.
//

#ifndef __final__SceneParser__
#define __final__SceneParser__

#include <iostream>
#include "algebra3.h"
#include "StaticObject.h"
#include <vector>
#include "Fluid.h"
using namespace std;

//struct to avoid having a lot of arguments passed around
struct SceneProperties {
  vec3 worldSize;
  float timestep;
  float smoothing;
};

//parses the scene and populate the necessary fields
class SceneParser {
  void parseScene(string fileName);
  public:
    SceneProperties properties;
    vector<StaticObject> objects;
    vector<FluidVolume> volumes;
    SceneParser(string fileName) {
      parseScene(fileName);
    }
  
};
#endif /* defined(__final__SceneParser__) */


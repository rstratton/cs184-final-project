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
using namespace std;
struct SceneProperties {
  vec3 worldSize;
  float timestep;
  float smoothing;
};

class SceneParser {
  void parseScene(string fileName);
  public:
    SceneProperties properties;
    SceneParser(string fileName) {
      parseScene(fileName);
    }
  
};
#endif /* defined(__final__SceneParser__) */


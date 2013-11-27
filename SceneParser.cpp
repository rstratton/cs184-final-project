//
//  SceneParser.cpp
//  final
//
//  Created by Kavan Sikand on 11/27/13.
//  Copyright (c) 2013 Kavan Sikand. All rights reserved.
//

#include "SceneParser.h"


void SceneParser::parseScene(string fileName) {
  properties.worldSize = vec3(600,400,200);
  properties.timestep  = .01;
  properties.smoothing = 10;
}
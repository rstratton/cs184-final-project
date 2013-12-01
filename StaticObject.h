//
//  StaticObject.h
//  final
//
//  Created by Kavan Sikand on 11/28/13.
//  Copyright (c) 2013 Kavan Sikand. All rights reserved.
//

#ifndef __final__StaticObject__
#define __final__StaticObject__
#include <iostream>
#include "algebra3.h"
#include <vector>
//this class defines the static objects in the scene the fluid will interact with.
using namespace std;
class StaticObject {
  vector<vector<vec3> > faces;
  vec3 mins = NULL;
  vec3 maxes = NULL;
  public:
    StaticObject(string filename, vec3 center);
};
#endif /* defined(__final__StaticObject__) */

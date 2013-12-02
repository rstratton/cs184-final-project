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
#include "Shape.h"
#include "BVH.h"
#include <vector>
//this class defines the static objects in the scene the fluid will interact with.
using namespace std;
class Ray;
class StaticObject {
  BVHNode* shapes = nullptr;
  vec3 mins = NULL;
  vec3 maxes = NULL;
  ShapeProperties props;
  public:
    std::vector<Shape*> faces;
    StaticObject(string filename, vec3 center);
    bool intersectsRay(Ray& p, Intersection* in);
    ~StaticObject() {
      if(shapes != nullptr)
        delete shapes;
      for(int i = 0; i < faces.size();i++) {
        delete (Triangle*)faces[i];
      }
    }
};
#endif /* defined(__final__StaticObject__) */



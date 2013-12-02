//
//  BVH.h
//  final
//
//  Created by Kavan Sikand on 10/15/13.
//  Copyright (c) 2013 Kavan Sikand. All rights reserved.
//

#ifndef __RayTracer__BVH__
#define __RayTracer__BVH__
#include "algebra3.h"

#include <iostream>
#include <vector>
class Ray;

//bounding volume hierarchy
#include "Shape.h"

class BoundingBox {
private:
  vec3 minPos;
  vec3 maxPos;
public:
  BoundingBox() { BoundingBox(vec3(0,0,0),vec3(0,0,0));};
  BoundingBox(vec3 min, vec3 max) : minPos(min), maxPos(max){
    for(int i = 0; i < 3; i++) { //ensure ordering
      if(minPos[i] > maxPos[i]) {
        float temp = minPos[i];
        minPos[i] = maxPos[i];
        maxPos[i]= temp;
      }
    }
  };
  bool doesIntersect(Ray& p);
  float center(int i) {
    return (maxPos[i]-minPos[i])/ 2;
  }
  BoundingBox operator+(BoundingBox bb);
};

class BVHNode {
public:
  BVHNode(std::vector <Shape*> shapes, int l);
  BVHNode* leftChild;
  BVHNode* rightChild;
  bool isLeaf;
  Shape* shape;
  ~BVHNode ();
  bool doesIntersect(Ray& p) { return bb.doesIntersect(p); };
  int level;
  bool operator()(Shape* a, Shape* b);
protected:
  BoundingBox bb;
  
};

#endif /* defined(__RayTracer__BVH__) */

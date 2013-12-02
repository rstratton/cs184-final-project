//
//  BVH.cpp
//  final
//
//  Created by Kavan Sikand on 11/30/13.
//  Copyright (c) 2013 Kavan Sikand. All rights reserved.
//
//http://www.eng.utah.edu/~cs6965/slides/06.pdf

#include "BVH.h"
#include "Shape.h"
#include <vector>
#include "Shape.h"
using namespace std;
BVHNode::BVHNode(vector <Shape*> shapes, int l) {
  //build the bvh from the given shapes
  level = l;
  leftChild=NULL;
  rightChild=NULL;
  //sort the array so we are dividing evenly along x, then evenly along y, then evenly along z. only do when the bou
  //  time_t startTime = time(NULL);
  //  sort(shapes.begin(), shapes.end(), *this);
  //  time_t endTime = time(NULL);
  //  double diff = difftime(endTime, startTime);
  //  printf("sorting time was: %d seconds. \n",(int)floor(diff));
  
  //for now it seems to be less efficient?
  if(shapes.size() > 1) {
    isLeaf = false;
    int half = floor(shapes.size()/2);
    leftChild = new BVHNode(vector<Shape*>(shapes.begin(), shapes.end() - half), l+1);
    if(shapes.size() > half) {
      rightChild = new BVHNode(vector<Shape*>(shapes.end()- half, shapes.end()), l+1);
      bb = leftChild->bb + rightChild->bb;
    } else {
      bb = leftChild->bb;
    }
  } else {
    shape = shapes.at(0);
    bb = *shape->boundingBox;
    isLeaf = true;
  }
}

bool BVHNode::operator()(Shape* a, Shape* b)
{
  int i = level % 3; //sort by x, then y, then z
  return a->centerCoordinate(i) < b->centerCoordinate(i);
}

BVHNode::~BVHNode(){
  if(!isLeaf) {
    if(!(leftChild == NULL)) {
      delete leftChild;
    }
    if(!(rightChild == NULL) ){
      delete rightChild;
    }
  }
}




bool BoundingBox::doesIntersect(Ray& p) {
  
  float mint, maxt, tymin, tymax;
  float a = 1/(p.direction[0] + .001);
  if (a >= 0) {
    mint = a*(minPos[0] - p.pos[0]);
    maxt = a*(maxPos[0] - p.pos[0]);
  } else {
    mint = a*(maxPos[0] - p.pos[0]);
    maxt = a*(minPos[0] - p.pos[0]);
  }
  
  a = 1/(p.direction[1] + .001);
  if (a >= 0) {
    tymin = a*(minPos[1] - p.pos[1]);
    tymax = a*(maxPos[1] - p.pos[1]);
  } else {
    tymin = a*(maxPos[1] - p.pos[1]);
    tymax = a*(minPos[1] - p.pos[1]);
  }
  if ((mint > tymax) || (tymin > maxt))
    return false;
  if (tymin > mint)
    mint = tymin;
  if (tymax < maxt)
    maxt = tymax;
  float tzmin, tzmax;
  a = 1/(p.direction[2] + .001);
  if (a >= 0) {
    tzmin = a*(minPos[2] - p.pos[2]);
    tzmax = a*(maxPos[2] - p.pos[2]);
  } else {
    tzmin = a*(maxPos[2] - p.pos[2]);
    tzmax = a*(minPos[2] - p.pos[2]);
  }
  if ((mint > tzmax) || (tzmin > maxt))
    return false;
  if (tzmin > mint)
    mint = tzmin;
  if (tzmax < maxt)
    maxt = tzmax;
  
  return ((mint < p.tMax) && (maxt > p.tMin));
}

BoundingBox BoundingBox::operator+(BoundingBox bb) {
  vec3 newMin;
  vec3 newMax;
  newMin = vec3(min(minPos[0],bb.minPos[0]),min(minPos[1],bb.minPos[1]), min(minPos[2],bb.minPos[2]));
  newMax = vec3(max(maxPos[0],bb.maxPos[0]),max(maxPos[1],bb.maxPos[1]), max(maxPos[2],bb.maxPos[2]));
  return BoundingBox(newMin, newMax);
}

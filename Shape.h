//
//  Shape.h
//  final
//
//  Created by Kavan Sikand on 11/30/13.
//  Copyright (c) 2013 Kavan Sikand. All rights reserved.
//

#ifndef __final__Shape__
#define __final__Shape__

#include <iostream>
#include "Color.h"
#include "algebra3.h"
#include <vector>
class BoundingBox;
struct ShapeProperties {
  vec3 attenuation;
  Color specular;
  float shininess;
  Color emission;
  Color diffuse;
};

class LocalGeo{
public:
  vec3 pos, normal;
  LocalGeo(vec3 p, vec3 n) : pos(p), normal(n) { }
};

class Intersection{
public:
  LocalGeo localGeo;
  ShapeProperties* properties;
  float t_value;
  
  Intersection(LocalGeo lg, ShapeProperties* s, float t) : localGeo(lg), properties(s), t_value(t) { }
};

class Ray{
public:
  vec4 pos;
  vec4 direction;
  float tMin, tMax;
  
  Ray(vec4 p, vec4 dir, float min, float max);
  friend Ray operator*(mat4 transform, const Ray& p);
};

class Shape {
public:
  virtual bool intersect(Ray& p, Intersection* intersectInfo) {printf("standard"); return false;};
  mat4 transformation;
  ShapeProperties properties;
  BoundingBox* boundingBox;
  float centerCoordinate(int i) ;
};


class Triangle: public Shape {
public:
  Triangle(std::vector<vec3>* v, int* i,  mat4 t, ShapeProperties p);
  ~Triangle();
  bool intersect(Ray& p, Intersection* intersectInfo);
  std::vector<vec3> vertices;

};
#endif /* defined(__final__Shape__) */

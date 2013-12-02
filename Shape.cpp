//
//  Shape.cpp
//  final
//
//  Created by Kavan Sikand on 11/30/13.
//  Copyright (c) 2013 Kavan Sikand. All rights reserved.
//

#include "Shape.h"
#include "BVH.h"
Ray::Ray(vec4 p, vec4 d, float min, float max){
  pos = p;
  direction = d;
  tMin = min;
  tMax = max;
}

Ray operator*(mat4 transform, const Ray& p){
  vec4 rotatedPos = transform*p.pos;
  vec4 rotatedDir = transform*p.direction;
  
  return Ray(rotatedPos, rotatedDir, p.tMin, p.tMax);
}

float Shape::centerCoordinate(int i) {
  return this->boundingBox->center(i);
}



Triangle::Triangle(std::vector<vec3>* v, int* i,  mat4 t, ShapeProperties p) {
  transformation = t;
  properties=p;
  vec3 min = vec3(transformation*v->at(i[0]));
  vec3 max = vec3(transformation*v->at(i[0]));
  for(int j = 0; j < 3; j++) {
    vec3 vert = transformation*v->at(i[j]);
    for(int k = 0; k < 3; k++) {
      if(vert[k] < min[k]) min[k] = vert[k];
      if(vert[k] > max[k]) max[k] = vert[k];
    }
    vertices.push_back(vert);
  }
  boundingBox = new BoundingBox(min,max);
};

Triangle::~Triangle() {
  if(boundingBox) { delete boundingBox;}
}

bool Triangle::intersect(Ray& p, Intersection *intersectionInfo) {
  Ray transP = p;
  vec3 va = vertices.at(0);
  vec3 vb = vertices.at(1);
  vec3 vc = vertices.at(2);
  float a = va[0]-vb[0];
  float b = va[1]-vb[1];
  float c = va[2]-vb[2];
  float d = va[0]-vc[0];
  float e = va[1]-vc[1];
  float f = va[2]-vc[2];
  float g = (float)transP.direction[0];
  float h = (float)transP.direction[1];
  float i = (float)transP.direction[2];
  float j = va[0]-(float)transP.pos[0];
  float k = va[1]-(float)transP.pos[1];
  float l = va[2]-(float)transP.pos[2];
  
  float akMinusjb = a*k - j*b;
  float jcMinusal = j*c - a*l;
  float blMinuskc = b*l - k*c;
  float eiMinushf = e*i - h*f;
  float gfMinusdi = g*f - d*i;
  float dhMinuseg = d*h - e*g;
  
  float M = a*(eiMinushf) + b*(gfMinusdi) + c*(dhMinuseg);
  if(M > pow(10.0f,-7) || M < -pow(10.0f,-7)) {
    intersectionInfo->t_value = -(f*(akMinusjb) + e*(jcMinusal) + d*(blMinuskc))/M;
  } else {
    float tTimesM =-(f*(akMinusjb) + e*(jcMinusal) + d*(blMinuskc));
    intersectionInfo->t_value=tTimesM*(1/M);
  }
  if(intersectionInfo->t_value < p.tMin || intersectionInfo->t_value > p.tMax) {
    return false;
  }
  
  float gamma = (i*(akMinusjb) + h*(jcMinusal) + g*(blMinuskc)) / M;
  if(gamma < 0 || gamma > 1) {
    return false;
  }
  
  float beta = (j*eiMinushf +k*gfMinusdi + l*dhMinuseg)/M;
  
  if(beta < 0 || beta > 1-gamma) {
    return false;
  }
  vec3 v1 = va - vb;
  vec3 v2= va - vc;
  vec3 norm = v2^v1;
  norm.normalize();
  if(p.direction*vec4(norm,0) < 0) {
    norm = -norm;
  }
  intersectionInfo->localGeo = LocalGeo(p.pos + p.direction*intersectionInfo->t_value, norm);
  
  return true;
}
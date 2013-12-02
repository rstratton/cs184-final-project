//
//  StaticObject.cpp
//  final
//
//  Created by Kavan Sikand on 11/28/13.
//  Copyright (c) 2013 Kavan Sikand. All rights reserved.
//

#include "StaticObject.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include "Shape.h"

StaticObject::StaticObject(string filename, vec3 center) {
  shapes = nullptr;
  faces = std::vector<Shape*>();
  //parse the obj file
  std::ifstream inpfile(filename.c_str());
  if(!inpfile.is_open()) {
    printf("Unable to open file %s", filename.c_str());
    exit(1);
  } else {
    std::string line;
    std::vector<vec3> vertices = std::vector<vec3>();
    while(inpfile.good()) {
      std::vector<std::string> splitline;
      std::string buf;
      std::getline(inpfile,line);
      std::stringstream ss(line);
      
      while (ss >> buf) {
        splitline.push_back(buf);
      }
      if(splitline.size() == 0)
        continue;
      
      //Ignore comments
      if(splitline.size()>0 && splitline[0][0] == '#') {
        continue;
      }
      else if(splitline[0] == "v"){
        vec3 v = vec3(atof(splitline[1].c_str()) + center[0], atof(splitline[2].c_str()) + center[1],atof(splitline[3].c_str()) + center[2]);
        if(mins == NULL)
          mins = v;
        if(maxes== NULL)
          maxes = v;
        for(int i = 0; i < 3 ; i ++) {
          if(v[i] < mins[i])
            mins[i] = v[i];
          if(v[i] > maxes[i])
            maxes[i] = v[i];
        }
        vertices.push_back(v);
      } else if(splitline[0] == "f") {
        //Will only be able to handle triangles
        
        string v1;
        istringstream liness(splitline[1].c_str());
        getline( liness, v1, '/' );
        
        string v2;
        istringstream liness2(splitline[2].c_str());
        getline( liness2, v2, '/' );
        
        string v3;
        istringstream liness3(splitline[3].c_str());
        getline( liness3, v3, '/' );
        
        int* indices = (int*)malloc(3*sizeof(int));
        //Subtract 1 as the convention for .obj files is that the first vertex is at position 1
        indices[0] =atoi(v1.c_str()) - 1;
        indices[1] =atoi(v2.c_str()) - 1;
        indices[2] =atoi(v3.c_str()) - 1;
        faces.push_back(new Triangle(&vertices, indices,identity3D(), props));
        free(indices);

      }
      else {
        //std::cerr << "Unknown command in current context: " << splitline[0] << std::endl;
      }
    }
    
    inpfile.close();
  }
  shapes = new BVHNode(faces, 0);
}



bool StaticObject::intersectsRay(Ray& p, Intersection* in) {
  //copy code from rayTracer...limit faces to be only triangles?
  //first check this object's bounding box (determined by the union of its triangle's bounding boxes)
  std::stack<BVHNode*> iterStack;
  BVHNode* node = shapes;
  while (!iterStack.empty() || !(node == NULL)) {
    if(node->doesIntersect(p)) {
      if(!(node->isLeaf)) {
        if(node->rightChild != NULL) {
          iterStack.push(node->rightChild);
        }
        if(node->leftChild != NULL) {
          iterStack.push(node->leftChild);
        }
      } else { // check for real intersection
        if(node->shape->intersect(p, in)) {
          return true;
        }
      }
    }
    if(!iterStack.empty()) {
      node = iterStack.top();
      iterStack.pop();
    } else {
      node = NULL;
    }
  }
  
  return false;
}

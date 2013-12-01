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

StaticObject::StaticObject(string filename, vec3 center) {
  //parse the obj file
  std::ifstream inpfile(filename.c_str());
  if(!inpfile.is_open()) {
    printf("Unable to open file %s", filename.c_str());
    exit(1);
  } else {
    std::string line;
    std::vector<vec3> vertices;
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
        std::vector<vec3> vec = std::vector<vec3>();
        for(int i = 1; i < splitline.size(); i++) {
          std::string v1;
          std::istringstream liness(splitline[i].c_str());
          getline( liness, v1, '/' );
          vec.push_back(vertices.at(atoi(v1.c_str())-1));
        }
        faces.push_back(vec);
      }
      else {
        //std::cerr << "Unknown command in current context: " << splitline[0] << std::endl;
      }
    }
    
    inpfile.close();
  }
}


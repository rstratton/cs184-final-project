//
//  SceneParser.cpp
//  final
//
//  Created by Kavan Sikand on 11/27/13.
//  Copyright (c) 2013 Kavan Sikand. All rights reserved.
//

#include "SceneParser.h"
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>

void SceneParser::parseScene(string fileName) {
  //default values
  objects = vector<StaticObject*>();
  volumes = vector<FluidVolume>();
  
  map<string, FluidProperties> definedFluids = map<string, FluidProperties>();
  std::ifstream inpfile(fileName.c_str());
  if(!inpfile.is_open()) {
    std::cout << "Unable to open file" << std::endl;
    exit(1);
  } else {
    std::string line;
    while(inpfile.good()) {
      std::vector<std::string> splitline;
      std::string buf;
      std::getline(inpfile,line);
      std::stringstream ss(line);
      
      while (ss >> buf) {
        splitline.push_back(buf);
      }
      if(splitline.size() == 0)
        continue; //ignore empty lines
      if(splitline[0][0] == '#')
        continue; //comments
      
      if(splitline[0] == "size") {
        properties.worldSize = vec3(atoi(splitline[1].c_str()),atoi(splitline[2].c_str()),atoi(splitline[3].c_str()));
      } else if(splitline[0] == "timestep") {
        properties.timestep = atof(splitline[1].c_str());
      } else if(splitline[0] == "smoothing") {
        properties.smoothing = atof(splitline[1].c_str());
      } else if(splitline[0] == "fluid") {
        Color c = Color(atof(splitline[7].c_str()),atof(splitline[8].c_str()),atof(splitline[9].c_str()), atof(splitline[10].c_str()));
        pair<string, FluidProperties> p = pair<string, FluidProperties>(splitline[1],FluidProperties(atof(splitline[2].c_str()), atof(splitline[3].c_str()), atof(splitline[4].c_str()), atof(splitline[5].c_str()), atof(splitline[6].c_str()), c));
        definedFluids.insert(p);
      } else if(splitline[0] == "volume") {
        //first make sure there exists a fluid with the given name
        if(definedFluids.find(splitline[1]) == definedFluids.end()) {
          printf("cannot find a fluid named %s \n", splitline[1].c_str());
          exit(1);
        }
        //now make the volume
        vector<string> sub(splitline.begin() + 3, splitline.end());
        FluidVolume fv = FluidVolume(definedFluids[splitline[1]], splitline[2], sub);
        volumes.push_back(fv);
      } else if(splitline[0] == "object") {
        vec3 center = vec3(atof(splitline[2].c_str()),atof(splitline[3].c_str()),atof(splitline[4].c_str()));
        
        objects.push_back(new StaticObject(splitline[1],center));
      } else {
        printf("unrecognized command: %s", splitline[0].c_str());
      }
    }
    inpfile.close();
  }
}
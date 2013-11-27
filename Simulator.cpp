//
//  Simulator.cpp
//  final
//
//  Created by Kavan Sikand on 11/21/13.
//  Copyright (c) 2013 Kavan Sikand. All rights reserved.
//

#include "Simulator.h"
//#define USE_ACCELERATION_STRUCTURES

using namespace std;

void Simulator::initialize() {
  allParticles = vector<Particle> ();
  
  cutoff = 2*smoothing;
  
  float max = fmax(fmax(worldSize[0],worldSize[1]),worldSize[2]);
  numGridCells = floorf(max / cutoff); //n
#ifdef USE_ACCELERATION_STRUCTURES

  particleGrid = vector< vector<vector<list<unsigned int> > > >();
  for(int i = 0; i < numGridCells; i++) {
    particleGrid.push_back(vector<vector<list<unsigned int> > >());
    for(int j = 0; j < numGridCells; j ++) {
      particleGrid[i].push_back(vector<list<unsigned int> >());
      for(int k = 0; k < numGridCells; k++) {
        particleGrid[i][j].push_back(list<unsigned int>());
      }
    }
  }
  nextParticleGrid = particleGrid;
#endif
  timestep = .01;
}

vector<Particle*> Simulator::getNeighborsForParticle(unsigned int i) {
  vector<Particle*> finalVector = vector<Particle*>();
  Particle p = allParticles[i];
#ifdef USE_ACCELERATION_STRUCTURES
  //using 3x3 array for now
  for(int x = max(p.gridPosition.x -1,0); x <= min(p.gridPosition.x + 1,numGridCells); x++) {
    for(int y = max(p.gridPosition.y -1,0); y <= min(p.gridPosition.y + 1,numGridCells); y++) {
      for(int z = max(p.gridPosition.z-1,0); z <= min(p.gridPosition.z + 1,numGridCells); z++) {
        for(std::list<unsigned int>::const_iterator iterator = particleGrid[x][y][z].begin(), end = particleGrid[x][y][z].end(); iterator != end; ++iterator) {
          finalVector.push_back(&allParticles[*iterator]);
        }
      }
    }
  }
#else
  for(int j = 0; j < allParticles.size(); j ++) {
    if((allParticles[j].position-p.position).length() < cutoff) {
      finalVector.push_back(&(allParticles[j]));
    }
  }
#endif

  return finalVector;
}


void Simulator::advanceTimeStep() {
  for(int i = 0; i < allParticles.size(); i++) {
    allParticles[i].calculateForces(getNeighborsForParticle(i));
  }
  vector<int> toDelete = vector<int>();
  for(int i = 0; i< allParticles.size(); i++) {
#ifdef USE_ACCELERATION_STRUCTURES
    particleGrid[allParticles[i].gridPosition.x][allParticles[i].gridPosition.y][allParticles[i].gridPosition.z].remove(i);
#endif
    allParticles[i].advanceTimeStep(timestep,numGridCells);
#ifdef USE_ACCELERATION_STRUCTURES
    if(allParticles[i].gridPosition.x >= 0 && allParticles[i].gridPosition.y >= 0
       && allParticles[i].gridPosition.z >= 0 && allParticles[i].gridPosition.x < numGridCells
       && allParticles[i].gridPosition.y < numGridCells && allParticles[i].gridPosition.z < numGridCells)
    {
      particleGrid[allParticles[i].gridPosition.x][allParticles[i].gridPosition.y][allParticles[i].gridPosition.z].push_back(i);
    } else {
      toDelete.push_back(i);
    }
#else
    if (!(allParticles[i].position[0] >= 0 && allParticles[i].position[1] >= 0
       && allParticles[i].position[2] >= 0 && allParticles[i].position[0] < worldSize[0]
       && allParticles[i].position[1] < worldSize[1] && allParticles[i].position[2] < worldSize[2])) {
      toDelete.push_back(i);
    }
      
#endif
  }
  //now delete the particles
  for(int i = 0; i < toDelete.size(); i++) {
    allParticles.erase(allParticles.begin()+ (toDelete[i] - i)); //-i to deal with the offset of erasing the previous ones
  }
  
}


void Simulator::printParticleGrid() {
  
#ifdef USE_ACCELERATION_STRUCTURES
  for(int y = 0; y < numGridCells; y++) {
    for(int x = 0; x < numGridCells; x++) {
      printf("%lu \t", particleGrid[x][y][0].size());
    }
    printf("\n");
  }
#endif
  if(allParticles.size() > 0) {
      printf("\n %f %f %f \n ",allParticles[0].position[0], allParticles[0].position[1], allParticles[0].position[2]);
  }

}

void Simulator::addParticle(vec3 pos, FluidProperties fp) {
  Particle p =  Particle(pos, fp, this);
  allParticles.push_back(p);
#ifdef USE_ACCELERATION_STRUCTURES
  particleGrid[p.gridPosition.x][p.gridPosition.y][p.gridPosition.z].push_back((unsigned int)allParticles.size()-1);
#endif
}


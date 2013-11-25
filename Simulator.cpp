//
//  Simulator.cpp
//  final
//
//  Created by Kavan Sikand on 11/21/13.
//  Copyright (c) 2013 Kavan Sikand. All rights reserved.
//

#include "Simulator.h"
using namespace std;

void Simulator::initialize() {
  particleGridCurrent = map<tuple<int, int, int>,list<Particle*> >();
  particleGridNext = map<tuple<int, int, int>,list<Particle*> >();
  allParticles = vector<Particle*> ();
  for(int i = 0; i < gridCells; i++) {
    for(int j = 0; j < gridCells; j ++) {
      for(int k = 0; k < gridCells; k++) {
        //method 1
        particleGridCurrent[tuple<int,int,int>(i,j,k)] = list<Particle*>();
        particleGridNext[tuple<int,int,int>(i,j,k)] = list<Particle*>();
        
        //method 2
        particleGrid[i][j][k] = list<Particle*>();
        nextParticleGrid[i][j][k] = list<Particle*>();

      }
    }
  }
    
  numTimesteps = 30;
  timestep = .1;
  
}

vector<Particle*> Simulator::getNeighborsForParticle(Particle* p) {
  vector<Particle*> finalVector = vector<Particle*>();
  
  //using 3x3 array for now
  for(int x = max(p->gridPosition.x -1,0); x <= min(p->gridPosition.x + 1,gridCells); x++) {
    for(int y = max(p->gridPosition.y -1,0); y <= min(p->gridPosition.y + 1,gridCells); y++) {
      for(int z = max(p->gridPosition.z-1,0); z <= min(p->gridPosition.z + 1,gridCells); z++) {
        for(std::list<Particle*>::const_iterator iterator = particleGrid[x][y][z].begin(), end = particleGrid[x][y][z].end(); iterator != end; ++iterator) {
          finalVector.push_back(*iterator);
        }
      }
    }
  }
  
  return finalVector;
}


void Simulator::advanceTimeStep() {
  for(int i = 0; i < allParticles.size(); i++) {
    allParticles[i]->calculateForces(getNeighborsForParticle(allParticles[i]));
  }
  std::vector<Particle*>::const_iterator iterator = allParticles.begin();
  while(iterator != allParticles.end()) {
    Particle* p = *iterator;
    particleGrid[p->gridPosition.x][p->gridPosition.y][p->gridPosition.z].remove(p);
    p->advanceTimeStep(gridCells, timestep);
    if(p->gridPosition.x >= 0 && p->gridPosition.y >= 0 && p->gridPosition.z >= 0
      && p->gridPosition.x < gridCells && p->gridPosition.y < gridCells && p->gridPosition.z < gridCells)
    {
      particleGrid[p->gridPosition.x][p->gridPosition.y][p->gridPosition.z].push_back(p);
      ++iterator;
    } else {
      iterator = allParticles.erase(iterator);
      delete p;
    }
  }
}

void Simulator::runSimulation() {
  for(int i = 0; i < numTimesteps; i++) {
    advanceTimeStep();
    printParticleGrid();
  }
}

void Simulator::printParticleGrid() {
  
  //using 3x3 array for now
  for(int y = 0; y <= gridCells; y++) {
    for(int x = 0; x <= gridCells; x++) {
      printf("%lu \t", particleGrid[x][y][0].size());
    }
    printf("\n");
  }
  
  printf("\n %f %f %f \n ",allParticles[0]->position[0], allParticles[0]->position[1], allParticles[0]->position[2]);
}

void Simulator::addParticle(Particle* p) {
  allParticles.push_back(p);
  particleGrid[p->gridPosition.x][p->gridPosition.y][p->gridPosition.z].push_back(allParticles.back());
}


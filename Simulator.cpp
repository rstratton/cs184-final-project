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
  allParticles = vector<Particle> ();
  for(int i = 0; i < gridCells; i++) {
    for(int j = 0; j < gridCells; j ++) {
      for(int k = 0; k < gridCells; k++) {
        particleGrid[i][j][k] = list<unsigned int>();
        nextParticleGrid[i][j][k] = list<unsigned int>();

      }
    }
  }
    
  numTimesteps = 30;
  timestep = .1;
  
}

vector<Particle*> Simulator::getNeighborsForParticle(unsigned int i) {
  vector<Particle*> finalVector = vector<Particle*>();
  Particle p = allParticles[i];
  //using 3x3 array for now
  for(int x = max(p.gridPosition.x -1,0); x <= min(p.gridPosition.x + 1,gridCells); x++) {
    for(int y = max(p.gridPosition.y -1,0); y <= min(p.gridPosition.y + 1,gridCells); y++) {
      for(int z = max(p.gridPosition.z-1,0); z <= min(p.gridPosition.z + 1,gridCells); z++) {
        for(std::list<unsigned int>::const_iterator iterator = particleGrid[x][y][z].begin(), end = particleGrid[x][y][z].end(); iterator != end; ++iterator) {
          finalVector.push_back(&allParticles[*iterator]);
        }
      }
    }
  }
  
  return finalVector;
}


void Simulator::advanceTimeStep() {
  for(int i = 0; i < allParticles.size(); i++) {
    allParticles[i].calculateForces(getNeighborsForParticle(i));
  }
  vector<int> toDelete = vector<int>();
  for(int i = 0; i< allParticles.size(); i++) {
    particleGrid[allParticles[i].gridPosition.x][allParticles[i].gridPosition.y][allParticles[i].gridPosition.z].remove(i);
    allParticles[i].advanceTimeStep(gridCells, timestep);
    if(allParticles[i].gridPosition.x >= 0 && allParticles[i].gridPosition.y >= 0
       && allParticles[i].gridPosition.z >= 0 && allParticles[i].gridPosition.x < gridCells
       && allParticles[i].gridPosition.y < gridCells && allParticles[i].gridPosition.z < gridCells)
    {
      particleGrid[allParticles[i].gridPosition.x][allParticles[i].gridPosition.y][allParticles[i].gridPosition.z].push_back(i);
    } else {
      toDelete.push_back(i);
    }
  }
  //now delete the particles
  for(int i = 0; i < toDelete.size(); i++) {
    allParticles.erase(allParticles.begin()+ (toDelete[i] - i)); //-i to deal with the offset of erasing the previous ones
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
  
  printf("\n %f %f %f \n ",allParticles[0].position[0], allParticles[0].position[1], allParticles[0].position[2]);
}

void Simulator::addParticle(Particle p) {
  allParticles.push_back(p);
  particleGrid[p.gridPosition.x][p.gridPosition.y][p.gridPosition.z].push_back((unsigned int)allParticles.size()-1);
}


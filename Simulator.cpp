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
  
}

void addToVector(vector<Particle*> *vect, list<Particle*> l) {
  for(std::list<Particle*>::const_iterator iterator = l.begin(), end = l.end(); iterator != end; ++iterator) {
    vect->push_back(*iterator);
  }
}

vector<Particle*> Simulator::getNeighborsForParticle(Particle p) {
  vector<Particle*> finalVector = vector<Particle*>();
  
  //using 3x3 array for now
  for(int x = p.gridPosition.x -1; x <= p.gridPosition.x + 1; x++) {
    for(int y = p.gridPosition.y -1; y <= p.gridPosition.y + 1; y++) {
      for(int z = p.gridPosition.z-1; z <= p.gridPosition.z + 1; z++) {
        addToVector(&finalVector,particleGrid[x][y][z]);
      }
    }
  }
  return finalVector;
}



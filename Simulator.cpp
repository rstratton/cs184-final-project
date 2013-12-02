//
//  Simulator.cpp
//  final
//
//  Created by Kavan Sikand on 11/21/13.
//  Copyright (c) 2013 Kavan Sikand. All rights reserved.
//

#include "Simulator.h"
//#define USE_ACCELERATION_STRUCTURES
#include "Shape.h"
using namespace std;

void Simulator::initialize() {
  allParticles = vector<Particle> ();
  
  //set up the fluid at the initial state
  for(int i = 0; i < volumes.size(); i ++) {
    volumes[i].populateFluid(this); //puts the particles evenly spaced through the fluid volume.
  }
  
  //figure out the size for the grid
  cutoff = 2*properties.smoothing;
  float max = fmax(fmax(properties.worldSize[0],properties.worldSize[1]),properties.worldSize[2]);
  numGridCells = floorf(max / cutoff); //n
  
  //initialize the grid
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
  //naive, just iterate and check the distance
  for(int j = 0; j < allParticles.size(); j ++) {
    if((allParticles[j].position-p.position).length() < cutoff) {
      finalVector.push_back(&(allParticles[j]));
    }
  }
#endif

  return finalVector;
}


void Simulator::advanceTimeStep() {
  //float GAS_CONST = 8.3145;
  //float GAS_CONST = pow(1.3806, -23);
  float GAS_CONST = fp.pressureConstant;
  float WATER_REST_DENSITY = 1000;
  for(int i = 0; i < allParticles.size(); i++) { //first loop to calculate pressure values for all particles
    allParticles[i].neighbors = getNeighborsForParticle(i);
    allParticles[i].pressure = GAS_CONST * (pow(allParticles[i].calculateDensity(allParticles[i].neighbors) / WATER_REST_DENSITY, 7) - 1);
  }
  for(int i = 0; i < allParticles.size(); i++) { //second loop to calculate new accelerations and forces
    allParticles[i].calculateForces(); 
  }
  vector<int> toDelete = vector<int>();
  //now actually move the particles
  for(int i = 0; i< allParticles.size(); i++) {
#ifdef USE_ACCELERATION_STRUCTURES
    particleGrid[allParticles[i].gridPosition.x][allParticles[i].gridPosition.y][allParticles[i].gridPosition.z].remove(i);
#endif
    //check for object intersections
    if(!checkObjectIntersection(i)) {
      //if no intersection, just move it normally
      allParticles[i].advanceTimeStep(properties.timestep,numGridCells);
    } else {
      printf("INTERSECTION \n \n \n ");
    }

    
    //delete any that went offscreen
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
       && allParticles[i].position[2] >= 0 && allParticles[i].position[0] < properties.worldSize[0]
       && allParticles[i].position[1] < properties.worldSize[1] && allParticles[i].position[2] < properties.worldSize[2])) {
      toDelete.push_back(i);
    }
      
#endif
  }
  //now delete the particles that went offscreen
  for(int i = 0; i < toDelete.size(); i++) {
    allParticles.erase(allParticles.begin()+ (toDelete[i] - i)); //-i to deal with the offset of erasing the previous ones
  }
  
}

Ray getReflectedRay(Ray r, Intersection* intersect) {
  vec4 inDir = r.direction;
  inDir.normalize();
  vec4 newDir = inDir - 2*(inDir*vec4(intersect->localGeo.normal,0))*vec4(intersect->localGeo.normal,0);
  vec4 pos = intersect->localGeo.pos;
  return Ray(pos,newDir,.001f,300);
}

bool Simulator::checkObjectIntersection(int i) {
#ifdef USE_ACCELERATION_STRUCTURES
  //have some optimized code here, perhaps AABB tree on the level of objects?
#else
  for(int j = 0; j < objects.size(); j++) {
    Ray r = Ray(allParticles[i].position,allParticles[i].velocity,0,properties.timestep);
    Intersection *in = (Intersection*) malloc(sizeof(Intersection));;
    if(objects[j]->intersectsRay(r, in)) {
      vec4 newDir = getReflectedRay(r, in).direction;
      newDir.normalize();
      newDir *= allParticles[i].fp.elasticity * allParticles[i].velocity.length() * ((properties.timestep-in->t_value) / properties.timestep);
      allParticles[i].velocity = newDir;
      return true;
    }
  }
#endif
  return false;
}


//temporary function for debugging/testing
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
    for(int i = 0; i < allParticles.size(); i++)
      printf("\tpoint: %f %f %f \t ",allParticles[i].position[0], allParticles[i].position[1], allParticles[i].position[2]);
    printf("\n \n");
  }

}

//create a new particle at this position
void Simulator::addParticle(vec3 pos, FluidProperties fp) {
  Particle p =  Particle(pos, fp, this);
  allParticles.push_back(p);
#ifdef USE_ACCELERATION_STRUCTURES
  particleGrid[p.gridPosition.x][p.gridPosition.y][p.gridPosition.z].push_back((unsigned int)allParticles.size()-1);
#endif
}

float Simulator::kernelFunction(vec3 difference) {
  if(difference.length() > 2*properties.smoothing) {
    return 0;
  }
  //using one from the paper, a gaussian.
  float term =1/(pow(PI,1.5)*pow(properties.smoothing,3.));
  float e = exp(pow(difference.length(),2.)/pow(properties.smoothing,2.));
  
  return term * e;
}

vec3 Simulator::pressureGradient(vec3 difference) {
  if(difference.length() > properties.smoothing) {
    return 0;
  }
  return -30/PI*pow(properties.smoothing,6.)*pow(properties.smoothing-difference.length,2.)*difference.normalize();
}

vec3 Simulator::viscosityGradient(vec3 difference) {
  if(difference.length() > properties.smoothing) {
    return 0;
  }
  return 45/PI*pow(properties.smoothing,6.)*(properties.smoothing-difference.length)*difference.normalize();
}

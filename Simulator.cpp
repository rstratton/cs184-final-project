//
//  Simulator.cpp
//  final
//
//  Created by Kavan Sikand on 11/21/13.
//  Copyright (c) 2013 Kavan Sikand. All rights reserved.
//

#include "Simulator.h"
#define USE_ACCELERATION_STRUCTURES
#include "Shape.h"
#include <thread>
using namespace std;

void Simulator::initialize() {
  allParticles = vector<Particle> ();
  
  
  //figure out the size for the grid
  cutoff = properties.smoothing;
  numGridCells = vec3(ceil(properties.worldSize[0] / cutoff),ceil(properties.worldSize[1] / cutoff),ceil(properties.worldSize[2] / cutoff));
  kernelCoeff = 1.56668147/pow(properties.smoothing,9.);
#if (defined(OSX) || defined(__APPLE__)) && defined(PARALLEL)
  tbb::task_scheduler_init init(std::thread::hardware_concurrency());
  printf("concurrency: %d \n", std::thread::hardware_concurrency());
  densityExec =  new TbbDensityExecutor((Simulator*)this);
  forceExec =  new TbbForceExecutor((Simulator*)this);
  moveExec =  new TbbMoveExecutor((Simulator*)this);
#endif
  
  //initialize the grid
#ifdef USE_ACCELERATION_STRUCTURES
  particleGrid = vector< vector<vector<list<unsigned int> > > >();
  for(int i = 0; i < numGridCells[0]; i++) {
    particleGrid.push_back(vector<vector<list<unsigned int> > >());
    for(int j = 0; j < numGridCells[1]; j ++) {
      particleGrid[i].push_back(vector<list<unsigned int> >());
      for(int k = 0; k < numGridCells[2]; k++) {
        particleGrid[i][j].push_back(list<unsigned int>());
      }
    }
  }
#endif
  
  //set up the fluid at the initial state
  for(int i = 0; i < volumes.size(); i ++) {
    volumes[i].populateFluid(this); //puts the particles evenly spaced through the fluid volume.
  }
}

float Simulator::calculateParticleDensity(int i, vector<int>* neighbors){
  float sum = 0;
  for(int j = 0; j < neighbors->size(); j++) {
    sum += allParticles[(*neighbors)[j]].fp->mass*kernelFunction(allParticles[i].position - allParticles[(*neighbors)[j]].position);
  }
  return sum;
}

void Simulator::calculateParticleForces(int i, vector<int>* neighbors)  {
  vec3 pressureForce = vec3(0);
  //comment out for now, just to test rendering
  if(allParticles[i].density > allParticles[i].fp->restDensity)
  {
    for(int j = 0; j < neighbors->size(); j++) {
      pressureForce += (allParticles[i].pressure + allParticles[(*neighbors)[j]].pressure)/2. * allParticles[(*neighbors)[j]].fp->mass/allParticles[(*neighbors)[j]].density * pressureGradient(allParticles[i].position - allParticles[(*neighbors)[j]].position);
    }
  }
  
  vec3 force = gravity*allParticles[i].density-pressureForce;// + viscosityForce + gravity*density;
  allParticles[i].acceleration = force/allParticles[i].density;
}

vector<int> Simulator::getNeighborsForParticle(unsigned int i) {
  vector<int> finalVector = vector<int>();
  Particle p = allParticles[i];
#ifdef USE_ACCELERATION_STRUCTURES
  //using 3x3 array for now
  for(int x = max(p.gridPosition.x -1,0); x <= fmin(p.gridPosition.x + 1.,numGridCells[0]-1); x++) {
    for(int y = max(p.gridPosition.y -1,0); y <= fmin(p.gridPosition.y + 1.,numGridCells[1]-1); y++) {
      for(int z = max(p.gridPosition.z-1,0); z <= fmin(p.gridPosition.z + 1.,numGridCells[2]-1); z++) {
        for(std::list<unsigned int>::const_iterator iterator = particleGrid[x][y][z].begin(), end = particleGrid[x][y][z].end(); iterator != end; ++iterator) {
          if((allParticles[i].position - allParticles[*iterator].position).length() < cutoff && *iterator != i) {
            finalVector.push_back(*iterator);
          }
          
        }
      }
    }
  }
#else
  //naive, just iterate and check the distance
  for(int j = 0; j < allParticles.size(); j ++) {
    if((allParticles[j].position-p.position).length() < cutoff && i != j) {
      finalVector.push_back(j);
    }
  }
#endif

  return finalVector;
}


void Simulator::moveParticleCalculation(int i) {
#ifdef USE_ACCELERATION_STRUCTURES
  particleGrid[allParticles[i].gridPosition.x][allParticles[i].gridPosition.y][allParticles[i].gridPosition.z].remove(i);
#endif
  //check for object intersections
  bool hit = checkObjectIntersection(i);
  //if no intersection, just move it normally
  if(!hit)
    allParticles[i].advanceTimeStep(properties.timestep,numGridCells);
}

void Simulator::densityCalculation(int i) {
  vector<int> neighbors = getNeighborsForParticle(i);
  allParticles[i].density = calculateParticleDensity(i, &neighbors);
  //    printf("density: %f \n", allParticles[i].density);
  
  allParticles[i].pressure = allParticles[i].fp->pressureConstant * (pow(allParticles[i].density / allParticles[i].fp->restDensity, 7) - 1);
}

void Simulator::forceCalculation(int i) {
  vector<int> neighbors = getNeighborsForParticle(i);
  calculateParticleForces(i,&neighbors);
}

vector<int> Simulator::getNeighborsForPosition(vec3 position) {
    GridPosition gridPosition;
    gridPosition.x = floor((position[0]/properties.worldSize[0])*numGridCells[0]);
    gridPosition.y = floor((position[1]/properties.worldSize[1])*numGridCells[1]);
    gridPosition.z = floor((position[2]/properties.worldSize[2])*numGridCells[2]);
    vector<int> finalVector = vector<int>();
#ifdef USE_ACCELERATION_STRUCTURES
    for(int x = max(gridPosition.x -1,0); x <= fmin(gridPosition.x + 1.,numGridCells[0]-1); x++) {
        for(int y = max(gridPosition.y -1,0); y <= fmin(gridPosition.y + 1.,numGridCells[1]-1); y++) {
            for(int z = max(gridPosition.z-1,0); z <= fmin(gridPosition.z + 1.,numGridCells[2]-1); z++) {
                for(std::list<unsigned int>::const_iterator iterator = particleGrid[x][y][z].begin(), end = particleGrid[x][y][z].end(); iterator != end; ++iterator) {
                    if((position - allParticles[*iterator].position).length() < cutoff) {
                        finalVector.push_back(*iterator);
                    }
                }
            }
        }
    }
#else
    for(int j = 0; j < allParticles.size(); j ++) {
        if((allParticles[j].position-position).length() < cutoff) {
            finalVector.push_back(j);
        }
    }
#endif
    return finalVector;
}


void Simulator::advanceTimeStep() {
  //float GAS_CONST = 8.3145;
  //float GAS_CONST = pow(1.3806, -23);
#if (defined(OSX) || defined(__APPLE__)) && defined(PARALLEL)
  parallel_for( tbb::blocked_range<int>( 0, (int)allParticles.size()), *densityExec);
#else
  for(int i = 0; i < allParticles.size(); i++) { //first loop to calculate pressure values for all particles
    densityCalculation(i);
  }
#endif
#if (defined(OSX) || defined(__APPLE__)) && defined(PARALLEL)
  parallel_for( tbb::blocked_range<int>( 0, (int)allParticles.size()), *forceExec);
#else
  for(int i = 0; i < allParticles.size(); i++) { //second loop to calculate new accelerations and forces
    vector<int> neighbors = getNeighborsForParticle(i);
    calculateParticleForces(i,&neighbors);
  }
#endif
  //empty the particle grid;
  clearParticleGrid();
 
  
#if (defined(OSX) || defined(__APPLE__)) && defined(PARALLEL)
  parallel_for( tbb::blocked_range<int>( 0, (int)allParticles.size()), *moveExec);
#else
  //now actually move the particles
  for(int i = 0; i < allParticles.size(); i++) {
    moveParticleCalculation(i); //not easy to parallelize because of data structure...worth it?
  }
#endif

  toDelete = vector<int>();
  for(int i = 0; i < allParticles.size(); i++) {
      //delete any that went offscreen
    #ifdef USE_ACCELERATION_STRUCTURES
      if(allParticles[i].gridPosition.x >= 0 && allParticles[i].gridPosition.y >= 0
         && allParticles[i].gridPosition.z >= 0 && allParticles[i].gridPosition.x < numGridCells[0]
         && allParticles[i].gridPosition.y < numGridCells[1] && allParticles[i].gridPosition.z < numGridCells[2])
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
  toDelete.clear();
  
}

Ray getReflectedRay(Ray r, Intersection* intersect) {
  vec4 inDir = r.direction;
  inDir.normalize();
  vec4 newDir = inDir - 2*(inDir*vec4(intersect->localGeo.normal,0))*vec4(intersect->localGeo.normal,0);
  vec4 pos = intersect->localGeo.pos;
  return Ray(pos,newDir,.001f,300);
}

vec3 getPlaneProjection(vec3 dir, vec3 normal) {
  float len = dir.length();
  dir.normalize();
  normal.normalize();
  vec3 projUnit = dir - (dir*normal) * normal;
  return projUnit*len;
}

bool Simulator::checkObjectIntersection(int i) {
  for(int j = 0; j < objects.size(); j++) {
    Ray r = Ray(allParticles[i].position,allParticles[i].velocity,0,properties.timestep);
    Intersection *in = (Intersection*) malloc(sizeof(Intersection));;
    if(objects[j]->intersectsRay(r, in)) {
//      vec3 vel = getPlaneProjection(allParticles[i].velocity, in->localGeo.normal  );
//      allParticles[i].position += vel;
//      allParticles[i].velocity = 0;
//      if(allParticles[i].position[1] < 5) {
//        printf("wrong dir vel: %f %f %f", vel[0],vel[1],vel[2]);
//      }
      return true;
    }
  }
  return false;
}


void Simulator::clearParticleGrid() {
  for(int x = 0; x < numGridCells[0]; x++) {
    for(int y = 0; y < numGridCells[1]; y++) {
      for(int z = 0; z < numGridCells[2]; z++) {
        particleGrid[x][y][z].clear();
      }
    }
  }
}

//temporary function for debugging/testing
void Simulator::printParticleGrid() {
  
#ifdef USE_ACCELERATION_STRUCTURES
  for(int y = 0; y < numGridCells[1]; y++) {
    for(int x = 0; x < numGridCells[0]; x++) {
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
void Simulator::addParticle(vec3 pos, FluidProperties* fp) {
  Particle p =  Particle(pos, fp, this);
  allParticles.push_back(p);
#ifdef USE_ACCELERATION_STRUCTURES
  particleGrid[p.gridPosition.x][p.gridPosition.y][p.gridPosition.z].push_back((unsigned int)allParticles.size()-1);
#endif
}

float Simulator::kernelFunction(vec3 difference) {
  if(difference.length() > properties.smoothing) {
    return 0;
  }
  return kernelCoeff*pow(pow(properties.smoothing,2.)-pow(difference.length(),2.),3.);
}

vec3 Simulator::pressureGradient(vec3 difference) {
  if(difference.length() > properties.smoothing) {
    return 0;
  }
  //use non-exponential kernal:
  float coeff = -14.32394488*pow(properties.smoothing-difference.length(),2.)/(pow(properties.smoothing,6.)*difference.length());
  return vec3(difference[0]*coeff,difference[1]*coeff,difference[2]*coeff);
}

float Simulator::viscosity(vec3 difference) {
  if(difference.length() > 2*properties.smoothing || difference.length() == 0) {
    return 0;
  }
  return (1.07752*pow(properties.smoothing,2.)+0.718348*pow(difference[0],2.)+0.718348*pow(difference[1],2.)+0.718348*pow(difference[2],2.))*exp((pow(difference[0],2.)+pow(difference[1],2.)+pow(difference[2],2.))/pow(properties.smoothing,2.))/pow(properties.smoothing,7.);
}

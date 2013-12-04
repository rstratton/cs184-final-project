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
  vec3 pressureForce = vec3();
  //comment out for now, just to test rendering
  for(int j = 0; j < neighbors->size(); j++) {
    pressureForce += (allParticles[i].pressure + allParticles[(*neighbors)[j]].pressure)/2. * allParticles[(*neighbors)[j]].fp->mass/allParticles[(*neighbors)[j]].density * pressureGradient(allParticles[i].position - allParticles[(*neighbors)[j]].position);
  }
  vec3 force = gravity*allParticles[i].density-pressureForce;// + viscosityForce + gravity*density;
  allParticles[i].acceleration = force/allParticles[i].density;
}

vector<int> Simulator::getNeighborsForParticle(unsigned int i) {
  vector<int> finalVector = vector<int>();
  Particle p = allParticles[i];
#ifdef USE_ACCELERATION_STRUCTURES
  //using 3x3 array for now
  for(int x = max(p.gridPosition.x -1,0); x <= min(p.gridPosition.x + 1,numGridCells); x++) {
    for(int y = max(p.gridPosition.y -1,0); y <= min(p.gridPosition.y + 1,numGridCells); y++) {
      for(int z = max(p.gridPosition.z-1,0); z <= min(p.gridPosition.z + 1,numGridCells); z++) {
        for(std::list<unsigned int>::const_iterator iterator = particleGrid[x][y][z].begin(), end = particleGrid[x][y][z].end(); iterator != end; ++iterator) {
          finalVector.push_back(*iterator);
        }
      }
    }
  }
#else
  //naive, just iterate and check the distance
  for(int j = 0; j < allParticles.size(); j ++) {

    if((allParticles[j].position-p.position).length() < cutoff) {
      finalVector.push_back(j);
    }
  }
#endif

  return finalVector;
}


void Simulator::advanceTimeStep() {
  //float GAS_CONST = 8.3145;
  //float GAS_CONST = pow(1.3806, -23);
  vector<vector<int>> neighbors = vector<vector<int>>();
  for(int i = 0; i < allParticles.size(); i++) { //first loop to calculate pressure values for all particles
    
    neighbors.push_back(getNeighborsForParticle(i));
    allParticles[i].density = calculateParticleDensity(i, &neighbors[i]);
//    printf("density: %f \n", allParticles[i].density);

    allParticles[i].pressure = allParticles[i].fp->pressureConstant * (pow(allParticles[i].density / allParticles[i].fp->restDensity, 7) - 1);
  }
  
  for(int i = 0; i < allParticles.size(); i++) { //second loop to calculate new accelerations and forces
    calculateParticleForces(i,&neighbors[i]);
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
  //using one from the paper, a gaussian.
  //float term =1/(pow(PI,1.5)*pow(properties.smoothing,3.));
  //float e = exp(pow(difference.length(),2.)/pow(properties.smoothing,2.));
  
  //return term * e;

  //using cubic kernal:
  //return 315/(64*PI*pow(properties.smoothing,9.))*pow(pow(properties.smoothing,2.)-pow(difference.length,2.),3.)
  return 1.56668147/pow(properties.smoothing,9.)*pow(pow(properties.smoothing,2.)-pow(difference.length(),2.),3.);
}

vec3 Simulator::pressureGradient(vec3 difference) {
  if(difference.length() > properties.smoothing) {
    return 0;
  }
  //vec3 grad(0.359174*difference[0]*exp((pow(difference[0],2.)+pow(difference[1],2.)+pow(difference[2],2.))/pow(properties.smoothing,2.))/pow(properties.smoothing,5.), 0.359174*difference[1]*exp((pow(difference[0],2.)+pow(difference[1],2.)+pow(difference[2],2.))/pow(properties.smoothing,2.))/pow(properties.smoothing,5.), 0.359174*difference[2]*exp((pow(difference[0],2.)+pow(difference[1],2.)+pow(difference[2],2.))/pow(properties.smoothing,2.))/pow(properties.smoothing,5.));
  //use non-exponential kernal:
  vec3 grad(14.32394488*difference[0]*pow(properties.smoothing-difference.length(),2.)/(pow(properties.smoothing,6.)*difference.length()),14.32394488*difference[1]*pow(properties.smoothing-difference.length(),2.)/(pow(properties.smoothing,6.)*difference.length()),14.32394488*difference[2]*pow(properties.smoothing-difference.length(),2.)/(pow(properties.smoothing,6.)*difference.length()));
  return grad;
  //return -30/PI*pow(properties.smoothing,6.)*pow(properties.smoothing-difference.length(),2.)*difference.normalize();
}

float Simulator::viscosity(vec3 difference) {
  if(difference.length() > 2*properties.smoothing || difference.length() == 0) {
    return 0;
  }
  return (1.07752*pow(properties.smoothing,2.)+0.718348*pow(difference[0],2.)+0.718348*pow(difference[1],2.)+0.718348*pow(difference[2],2.))*exp((pow(difference[0],2.)+pow(difference[1],2.)+pow(difference[2],2.))/pow(properties.smoothing,2.))/pow(properties.smoothing,7.);
}

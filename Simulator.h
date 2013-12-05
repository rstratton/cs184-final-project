//
//  Simulator.h
//  final
//
//  Created by Kavan Sikand on 11/21/13.
//  Copyright (c) 2013 Kavan Sikand. All rights reserved.
//

#ifndef __final__Simulator__
#define __final__Simulator__
#include <iostream>
#include "algebra3.h"
#include "Particle.h"
#include <math.h>
#include <vector>
#include <list>
#include <map>
#include <utility>
#include "SceneParser.h"
#include "Fluid.h"
#define PI 3.14159

using namespace std;


class Simulator {


  //talked to the professor, seems like a list of int indices is the best method.
  vector<vector<vector<list<unsigned int> > > > particleGrid;
  vector<vector<vector<list<unsigned int> > > > nextParticleGrid;
  vector<FluidVolume> volumes; //the initial volumes the fluids have. Really only needed for initialization

  
  public:
    int cutoff;
    SceneProperties properties;
    //the objects the fluid will interact with
    vector <StaticObject*> objects;
    vec3 numGridCells;    //calculated at initialization
    void initialize();
    void addParticle(vec3 pos, FluidProperties* fp);
    void advanceTimeStep();
    void printParticleGrid();
    Simulator(SceneProperties p, vector<StaticObject*> obj, vector<FluidVolume> fv) : volumes(fv), properties(p) , objects(obj){};
  
    //ability to initialize an empty one
    Simulator() : volumes(vector<FluidVolume>()), properties(SceneProperties()) , objects(vector<StaticObject*>()){};
    ~Simulator() {
      for(int i = 0; i < objects.size(); i++) {
        delete objects[i];
      }
      objects.clear();
    }
    vector<int> getNeighborsForPosition(vec3 position);


  protected:
    friend struct Particle; //so particle can use these methods
    friend class Renderer;
    friend class ParticleInspector;
    float kernelFunction(vec3 difference); //simple gaussian for now
    vec3 pressureGradient(vec3 difference);
    float viscosity(vec3 difference);
    vector<Particle> allParticles;


  private:
    vector<int> getNeighborsForParticle(unsigned int i); //return a list of indices
    float calculateParticleDensity(int i, vector<int>* neighbors);
    void  calculateParticleForces(int i, vector<int>* neighbors);
    bool checkObjectIntersection(int i);
  float kernelCoeff;
  
};

#endif /* defined(__final__Simulator__) */

#include <vector>
#include <math.h>
#include "Surface.h"
#include "Simulator.h"
using namespace std;

// How many samples from the scalar field will be taken per grid cell width
// (grid cells being the cells of the acceleration structure).  Grid cell width
// is equivalent to sim->cutoff
const int samplesPerGridCellWidth = 1;

Surface::Surface(Simulator* s) {
    sim = s;
    latticePointSpacing = sim->cutoff / float(samplesPerGridCellWidth);
    xSamples = ceil(sim->properties.worldSize[0] / latticePointSpacing);
    ySamples = ceil(sim->properties.worldSize[1] / latticePointSpacing);
    zSamples = ceil(sim->properties.worldSize[2] / latticePointSpacing);

    // Initialize lattice points 3D array
    latticePoints = new LatticePoint**[xSamples];
    for (int i = 0; i < xSamples; ++i) {
        latticePoints[i] = new LatticePoint*[ySamples];
        for (int j = 0; j < ySamples; ++j) {
            latticePoints[i][j] = new LatticePoint[zSamples];
            for (int k = 0; k < zSamples; ++k) {
                vec3 location(i * latticePointSpacing,
                              j * latticePointSpacing,
                              k * latticePointSpacing);
                // TODO: Sample field here and assign inclusion values
                // Does this necessitate constructing a particle to get
                // neighbors?
            }
        }
    }
}

//float Simulator::calculateParticleDensity(int i, vector<int>* neighbors){
//  float sum = 0;
//  for(int j = 0; j < neighbors->size(); j++) {
//    sum += allParticles[(*neighbors)[j]].fp->mass*kernelFunction(allParticles[i].position - allParticles[(*neighbors)[j]].position);
//  }
//  return sum;
//}

#include <vector>
#include <math.h>
#include "Surface.h"
#include "Simulator.h"
using namespace std;

// How many samples from the scalar field will be taken per grid cell width
// (grid cells being the cells of the acceleration structure).  Grid cell width
// is equivalent to sim->cutoff
const int samplesPerGridCellWidth = 5;

Surface::Surface(Simulator* s) {
    sim = s;
    latticePointSpacing = sim->cutoff / float(samplesPerGridCellWidth);
    xSamples = ceil(sim->properties.worldSize[0] / latticePointSpacing);
    ySamples = ceil(sim->properties.worldSize[1] / latticePointSpacing);
    zSamples = ceil(sim->properties.worldSize[2] / latticePointSpacing);

    // Initialize lattice points 3D array with position and inclusion fields
    // for each LatticePoint
    latticePoints = new LatticePoint**[xSamples];
    for (int i = 0; i < xSamples; ++i) {
        latticePoints[i] = new LatticePoint*[ySamples];
        for (int j = 0; j < ySamples; ++j) {
            latticePoints[i][j] = new LatticePoint[zSamples];
            for (int k = 0; k < zSamples; ++k) {
                latticePoints[i][j][k].position = vec3(i * latticePointSpacing,
                                                       j * latticePointSpacing,
                                                       k * latticePointSpacing);
                latticePoints[i][j][k].included = positionIsInSurface(latticePoints[i][j][k].position);
            }
        }
    }
}

Surface::~Surface() {
    for (int i = 0; i < xSamples; ++i) {
        for (int j = 0; j < ySamples; ++j) {
            delete latticePoints[i][j];
        }
        delete latticePoints[i];
    }
}

void Surface::resample() {
    for (int i = 0; i < xSamples; ++i) {
        for (int j = 0; j < ySamples; ++j) {
            for (int k = 0; k < zSamples; ++k) {
                latticePoints[i][j][k].included = positionIsInSurface(latticePoints[i][j][k].position);
            }
        }
    }
}

bool Surface::positionIsInSurface(vec3 position) {
    vector<int> neighbors = sim->getNeighborsForPosition(position);
    float sum = 0;
    for(int j = 0; j < neighbors.size(); j++) {
        Particle neighbor = sim->allParticles[neighbors[j]];
        sum += (neighbor.fp->mass / neighbor.density ) * sim->kernelFunction(position - neighbor.position);
    }
    return sum > 0.5;
}

#ifndef __final__Surface__
#define __final__Surface__

#include <vector>
#include "Simulator.h"

struct LatticePoint {
    LatticePoint() : position(), included(false) {};
    LatticePoint(vec3 loc, bool inc) : position(loc), included(inc) {};
    vec3 position;
    bool included;
};

class Surface {
    public:
        Simulator *sim;
        LatticePoint*** latticePoints;
        // Unit distance between samples of the scalar field
        float latticePointSpacing;
        // The number of lattice point samples along X, Y, and Z axes
        int xSamples, ySamples, zSamples;

        Surface(Simulator* s);
        ~Surface();
        void resample();
        bool positionIsInSurface(vec3 position);
};

#endif

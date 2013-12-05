#ifndef __final__Surface__
#define __final__Surface__

#include <vector>
#include "Simulator.h"

struct LatticePoint {
    LatticePoint() : location(), inclusion(false) {};
    LatticePoint(vec3 loc, bool inc) : location(loc), inclusion(inc) {};
    vec3 location;
    bool inclusion;
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
};

#endif

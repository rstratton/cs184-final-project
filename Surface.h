#ifndef __final__Surface__
#define __final__Surface__

#include <vector>
#include <GLUT/glut.h>
#include "Simulator.h"
#include "algebra3.h"

struct MeshTriangle {
    vec3 a, b, c;
    MeshTriangle(vec3 a, vec3 b, vec3 c) : a(a), b(b), c(c) {};
    void drawOutline() {
        glVertex3f(a[VX], a[VY], a[VZ]);
        glVertex3f(b[VX], b[VY], b[VZ]);

        glVertex3f(b[VX], b[VY], b[VZ]);
        glVertex3f(c[VX], c[VY], c[VZ]);

        glVertex3f(c[VX], c[VY], c[VZ]);
        glVertex3f(a[VX], a[VY], a[VZ]);
    }

    void drawFace() {
        vec3 normal = ((a - b) ^ (a - c)).normalize();
        glNormal3f(normal[VX], normal[VY], normal[VZ]);
        glVertex3f(a[VX], a[VY], a[VZ]);
        glVertex3f(b[VX], b[VY], b[VZ]);
        glVertex3f(c[VX], c[VY], c[VZ]);
    }
};

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
        // The lookup table for how to polygonalize a tetrahedron given whether
        // its vertices are included or not
        std::vector< std::vector< std::vector<int> > > triLookup;

        Surface(Simulator* s);
        ~Surface();
        void resample();
        bool positionIsInSurface(vec3 position);
        std::vector<MeshTriangle>* getMesh();
        void initLookupTable();
        void appendTriangles(LatticePoint& a, LatticePoint& b, LatticePoint& c,
                             LatticePoint& d, bool reflected, vector<MeshTriangle>* triangles);
};

#endif

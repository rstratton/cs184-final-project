#include <iostream>
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
    initLookupTable();
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
    //latticePoints[20][20][20].included = true;
    //latticePoints[20][19][20].included = true;
    //latticePoints[21][19][20].included = true;
    //latticePoints[21][20][20].included = true;

    //latticePoints[20][20][21].included = true;
    //latticePoints[20][19][21].included = true;
    //latticePoints[21][19][21].included = true;
    //latticePoints[21][20][21].included = true;
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

// Using a marching tetrahedra method
vector<MeshTriangle>* Surface::getMesh() {
    vector<MeshTriangle>* triangles = new vector<MeshTriangle>();
    for (int i = 0; i < xSamples - 1; ++i) {
        for (int j = 0; j < ySamples - 1; ++j) {
            for (int k = 0; k < zSamples - 1; ++k) {
                // Split this cell (formed by 8 lattice points) into 6 tetrahedra
                // and append the triangles returned by the marching tetrahedra
                // algorithm to the triangles vector

                appendTriangles(latticePoints[i][j+1][k],
                                latticePoints[i][j][k],
                                latticePoints[i][j][k+1],
                                latticePoints[i+1][j][k+1],
                                false, triangles);

                appendTriangles(latticePoints[i][j+1][k],
                                latticePoints[i][j][k],
                                latticePoints[i+1][j][k],
                                latticePoints[i+1][j][k+1],
                                true, triangles);

                appendTriangles(latticePoints[i][j+1][k],
                                latticePoints[i][j+1][k+1],
                                latticePoints[i][j][k+1],
                                latticePoints[i+1][j][k+1],
                                true, triangles);

                appendTriangles(latticePoints[i][j+1][k],
                                latticePoints[i][j+1][k+1],
                                latticePoints[i+1][j+1][k+1],
                                latticePoints[i+1][j][k+1],
                                false, triangles);

                appendTriangles(latticePoints[i][j+1][k],
                                latticePoints[i+1][j+1][k],
                                latticePoints[i+1][j][k],
                                latticePoints[i+1][j][k+1],
                                false, triangles);

                appendTriangles(latticePoints[i][j+1][k],
                                latticePoints[i+1][j+1][k],
                                latticePoints[i+1][j+1][k+1],
                                latticePoints[i+1][j][k+1],
                                true, triangles);
            }
        }
    }
    return triangles;
}

// Given 4 points of a tetrahedron (in a specific order), append the triangles
// returned by the marching tetrahedra algorithm to the triangles vector
void Surface::appendTriangles(LatticePoint& a, LatticePoint& b, LatticePoint& c,
                              LatticePoint& d, bool reflected, vector<MeshTriangle>* triangles) {
    // list of midpoints of each edge of the tetrahedron
    vector<vec3> midpoints;
    midpoints.push_back((0.5 * a.position) + (0.5 * b.position));
    midpoints.push_back((0.5 * b.position) + (0.5 * c.position));
    midpoints.push_back((0.5 * c.position) + (0.5 * a.position));
    midpoints.push_back((0.5 * a.position) + (0.5 * d.position));
    midpoints.push_back((0.5 * b.position) + (0.5 * d.position));
    midpoints.push_back((0.5 * c.position) + (0.5 * d.position));

    // Bit vector (int value) used for lookup in polygonalization table
    int bitVector = (a.included << 3) | (b.included << 2) | (c.included << 1) | d.included;

    // If the specified tetrahedron is a mirror of the canonical tetrahedron
    // used in this algorithm, flip the bits in bitVector so that the mirror
    // image solution will be returned (guarantees that polygon vertices will
    // be specified in counter-clockwise order)
    if (reflected) bitVector ^= 0xf;
    if (bitVector == 0x0 || bitVector == 0xf) return;

    vector< vector<int> > triList = triLookup[bitVector];
    for (int i = 0; i < triList.size(); ++i) {
        vector<int> triPoints = triList[i];
        int m1 = triPoints[0];
        int m2 = triPoints[1];
        int m3 = triPoints[2];
        triangles->push_back(MeshTriangle(
                    midpoints[m1],
                    midpoints[m2],
                    midpoints[m3]));
    }
}

// Initialize the tetrahedron polygonalization lookup table
void Surface::initLookupTable() {
    //triLookup.reserve(16);

    // Populate lookup for 0000
    triLookup.push_back(vector< vector<int> >());
    //triLookup[0] = vector< vector<int> >();

    // Populate lookup for 0001
    triLookup.push_back(vector< vector<int> >());
    //triLookup[1] = vector< vector<int> >();

    triLookup[1].push_back(vector<int>());
    triLookup[1][0].push_back(3);
    triLookup[1][0].push_back(5);
    triLookup[1][0].push_back(4);

    // Populate lookup for 0010
    triLookup.push_back(vector< vector<int> >());
    //triLookup[2] = vector< vector<int> >();

    triLookup[2].push_back(vector<int>());
    triLookup[2][0].push_back(2);
    triLookup[2][0].push_back(1);
    triLookup[2][0].push_back(5);

    // Populate lookup for 0011
    triLookup.push_back(vector< vector<int> >());
    //triLookup[3] = vector< vector<int> >();

    triLookup[3].push_back(vector<int>());
    triLookup[3][0].push_back(3);
    triLookup[3][0].push_back(2);
    triLookup[3][0].push_back(1);

    triLookup[3].push_back(vector<int>());
    triLookup[3][1].push_back(3);
    triLookup[3][1].push_back(1);
    triLookup[3][1].push_back(4);

    // Populate lookup for 0100
    triLookup.push_back(vector< vector<int> >());
    //triLookup[4] = vector< vector<int> >();

    triLookup[4].push_back(vector<int>());
    triLookup[4][0].push_back(0);
    triLookup[4][0].push_back(4);
    triLookup[4][0].push_back(1);

    // Populate lookup for 0101
    triLookup.push_back(vector< vector<int> >());
    //triLookup[5] = vector< vector<int> >();

    triLookup[5].push_back(vector<int>());
    triLookup[5][0].push_back(0);
    triLookup[5][0].push_back(3);
    triLookup[5][0].push_back(5);

    triLookup[5].push_back(vector<int>());
    triLookup[5][1].push_back(0);
    triLookup[5][1].push_back(5);
    triLookup[5][1].push_back(1);

    // Populate lookup for 0110
    triLookup.push_back(vector< vector<int> >());
    //triLookup[6] = vector< vector<int> >();

    triLookup[6].push_back(vector<int>());
    triLookup[6][0].push_back(2);
    triLookup[6][0].push_back(0);
    triLookup[6][0].push_back(4);

    triLookup[6].push_back(vector<int>());
    triLookup[6][1].push_back(2);
    triLookup[6][1].push_back(4);
    triLookup[6][1].push_back(5);

    // Populate lookup for 0111
    triLookup.push_back(vector< vector<int> >());
    //triLookup[7] = vector< vector<int> >();

    triLookup[7].push_back(vector<int>());
    triLookup[7][0].push_back(2);
    triLookup[7][0].push_back(0);
    triLookup[7][0].push_back(3);

    // Populate lookup for 1000
    triLookup.push_back(vector< vector<int> >());
    //triLookup[8] = vector< vector<int> >();

    triLookup[8].push_back(vector<int>());
    triLookup[8][0].push_back(3);
    triLookup[8][0].push_back(0);
    triLookup[8][0].push_back(2);

    // Populate lookup for 1001
    triLookup.push_back(vector< vector<int> >());
    //triLookup[9] = vector< vector<int> >();

    triLookup[9].push_back(vector<int>());
    triLookup[9][0].push_back(0);
    triLookup[9][0].push_back(2);
    triLookup[9][0].push_back(5);

    triLookup[9].push_back(vector<int>());
    triLookup[9][1].push_back(0);
    triLookup[9][1].push_back(5);
    triLookup[9][1].push_back(4);

    // Populate lookup for 1010
    triLookup.push_back(vector< vector<int> >());
    //triLookup[10] = vector< vector<int> >();

    triLookup[10].push_back(vector<int>());
    triLookup[10][0].push_back(3);
    triLookup[10][0].push_back(0);
    triLookup[10][0].push_back(1);

    triLookup[10].push_back(vector<int>());
    triLookup[10][1].push_back(3);
    triLookup[10][1].push_back(1);
    triLookup[10][1].push_back(5);

    // Populate lookup for 1011
    triLookup.push_back(vector< vector<int> >());
    //triLookup[11] = vector< vector<int> >();

    triLookup[11].push_back(vector<int>());
    triLookup[11][0].push_back(0);
    triLookup[11][0].push_back(1);
    triLookup[11][0].push_back(4);

    // Populate lookup for 1100
    triLookup.push_back(vector< vector<int> >());
    //triLookup[12] = vector< vector<int> >();

    triLookup[12].push_back(vector<int>());
    triLookup[12][0].push_back(2);
    triLookup[12][0].push_back(3);
    triLookup[12][0].push_back(4);

    triLookup[12].push_back(vector<int>());
    triLookup[12][1].push_back(2);
    triLookup[12][1].push_back(4);
    triLookup[12][1].push_back(1);

    // Populate lookup for 1101
    triLookup.push_back(vector< vector<int> >());
    //triLookup[13] = vector< vector<int> >();

    triLookup[13].push_back(vector<int>());
    triLookup[13][0].push_back(2);
    triLookup[13][0].push_back(5);
    triLookup[13][0].push_back(1);

    // Populate lookup for 1110
    triLookup.push_back(vector< vector<int> >());
    //triLookup[14] = vector< vector<int> >();

    triLookup[14].push_back(vector<int>());
    triLookup[14][0].push_back(3);
    triLookup[14][0].push_back(4);
    triLookup[14][0].push_back(5);

    // Populate lookup for 1111
    triLookup.push_back(vector< vector<int> >());
    //triLookup[15] = vector< vector<int> >();

    //cout << triLookup.size() << endl;
    //for (int i = 0; i < triLookup.size(); ++i) {
    //    cout << "triLookup[" << i << "] = [";
    //    for (int j = 0; j < triLookup[i].size(); ++j) {
    //        cout << "(" << triLookup[i][j][0] << ","
    //                    << triLookup[i][j][1] << ","
    //                    << triLookup[i][j][2] << "), ";
    //    }
    //    cout << "]" << endl;
    //}
}

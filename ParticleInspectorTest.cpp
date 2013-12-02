#include <GLUT/glut.h>
#include "ParticleInspector.h"
using namespace std;

int main(int argc, char** argv) {
    Simulator* sim = new Simulator(SceneProperties(), vector<StaticObject*>(), vector<FluidVolume>());
    sim->initialize();
    vector<Particle>* particles = new vector<Particle>;
    particles->push_back(Particle(vec3(0, 0, 0), FluidProperties(), sim));
    particles->push_back(Particle(vec3(1, 0, 0), FluidProperties(), sim));
    particles->push_back(Particle(vec3(0, 1, 0), FluidProperties(), sim));
    particles->push_back(Particle(vec3(0, 0, 1), FluidProperties(), sim));
    particles->push_back(Particle(vec3(1, 1, 0), FluidProperties(), sim));
    particles->push_back(Particle(vec3(1, 0, 1), FluidProperties(), sim));
    particles->push_back(Particle(vec3(0, 1, 1), FluidProperties(), sim));
    particles->push_back(Particle(vec3(1, 1, 1), FluidProperties(), sim));
    ParticleInspector p(sim);
    p.run(argc, argv);
    delete sim;
}

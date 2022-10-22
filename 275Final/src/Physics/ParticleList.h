#pragma once
#include "Particle.h"
#include <vector>

struct ParticleList {
public:
	int activeParticles;
	int inactiveParticles;
	std::vector<Particle*> particles;
	std::vector<int> inactiveStack;
};

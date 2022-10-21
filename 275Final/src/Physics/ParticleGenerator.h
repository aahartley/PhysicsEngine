#pragma once
#include "Particle.h"
#include <vector>
#include <random>

class ParticleGenerator {
public:
	std::vector<Particle*> particles;
	float rate;
	float n;
	float f;
	Vec2 location{ 0,60 };

	ParticleGenerator(int n);
	Vec2 randomVel();
	void generateParticles(std::vector<Particle*>& particles, float t, float h);
};
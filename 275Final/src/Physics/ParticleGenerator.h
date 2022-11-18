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
	Vec2 location;
	std::vector<Vec2> pos;
	std::vector<int> colors;

	int numOfP = 0;
	int max;

	ParticleGenerator(Vec2 location);
	Vec2 randomVel();
	void generateParticles(std::vector<Particle*>& particles, float t, float h);
};
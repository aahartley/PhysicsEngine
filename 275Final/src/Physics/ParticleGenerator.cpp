#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(int n) :n(n) {
	rate = 1;
	f = 0;

}

Vec2 ParticleGenerator::randomVel() {
	std::random_device rd;
	std::mt19937 gen(rd());


	// instance of class std::normal_distribution with specific mean and stddev
	std::normal_distribution<float> d(3, 2);
	float x = std::max(-5.0f,std::min(d(gen), 5.0f));

	float y = std::max(-5.0f, std::min(d(gen), 5.0f));
	return { x,y };
}

void ParticleGenerator::generateParticles(std::vector<Particle*>& particles, float t, float h) {
	n = rate * h;
	f = f + (rate * h - n);
	if (f > 1) {
		n = n + 1;
		f = f - 1;
	}
	for (int i = 0; i < n; i++) {
		Particle* p = new Particle(location.x, location.y, 1, 1);
		p->velocity = randomVel();
		particles.push_back(p);
	}
}

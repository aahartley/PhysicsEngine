#include "ParticleGenerator.h"
#include "../Output.h"

ParticleGenerator::ParticleGenerator(Vec2 location) {
	this->location = location;
	n = 0;
	rate = 10000;
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
	max = pos.size();
	if (outputTime(t)) {
		n = rate * h;
		f = f + (rate * h - n);
		if (f > 1) {
			n = n + 1;
			f = f - 1;
		}
		for (int i = 0; i < n; i++) {
			if (numOfP <= max - 1) {
				Particle* p = new Particle(location.x, location.y, 1, 0.3);
				particles.push_back(p);
				p->endPos = pos.at(numOfP);
				p->color = colors.at(numOfP);
				p->velocity = randomVel();
				numOfP++;
			}

		}
	}
}

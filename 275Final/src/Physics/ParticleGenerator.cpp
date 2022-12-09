#include "ParticleGenerator.h"
#include "Constants.h"

ParticleGenerator::ParticleGenerator(Vec2 location, int rate) {
	this->location = location;
	n = 0;
	this->rate = rate;
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

void ParticleGenerator::generateParticles(std::vector<Particle*>& particles, float t, float h, int start, int end) {
	max = pos.size();
	if (t>=start && t<end) {
		n = rate * h;
		f = f + (rate * h - n);
		if (f > 1) {
			n = n + 1;
			f = f - 1;
		}
		if (start == 0) {
			for (int i = 0; i < n; i++) {
				if (numOfP <= max - 1) {
					Particle* p = new Particle(location.x, location.y, 1, 0.3);
					particles.push_back(p);
					p->endPos = pos.at(numOfP);
					p->color = colors.at(numOfP);
					if (keepStatic)p->alive = false;
					//p->velocity = randomVel();
					numOfP++;
				}

			}
		}
		else {
			for (int i = 0; i < n; i++) {
				if (numOfP <= max - 1) {
					Particle* p = new Particle(pos.at(numOfP).x, pos.at(numOfP).y, 1, 0.3);
					particles.push_back(p);
					p->endPos = pos.at(numOfP);
					p->color = colors.at(numOfP);
					if (keepStatic)p->alive = false;

					p->velocity = randomVel();
					numOfP++;
				}

			}
		}
	}

}
void ParticleGenerator::generateFireworks(std::vector<Particle*>& particles, float t, float h, int start, int end) {
	if (t >= start && t < end) {
		n = rate * h;
		f = f + (rate * h - n);
		if (f > 1) {
			n = n + 1;
			f = f - 1;
		}
		for (int i = 0; i < n; i++) {
			Particle* p = new Particle(location.x+randomFloat(-100,100), location.y - 30, 1, 0.3);
			p->velocity = Vec2(0, randomFloat(50,100));
			p->color = 0xFF4B53A8;
			p->particleType = FIREWORK;
			particles.push_back(p);
		}

	}

}
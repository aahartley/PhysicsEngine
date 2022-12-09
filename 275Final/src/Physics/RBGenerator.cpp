#include "RBGenerator.h"
#include "../Output.h"

RBGenerator::RBGenerator() {
	n = 0;
	rate = 4;
	f = 0;
	max = 50;

}

Vec2 RBGenerator::randomPos() {
	std::random_device rd;
	std::mt19937 gen(rd());


	// instance of class std::normal_distribution with specific mean and stddev
	std::normal_distribution<float> d(60, 20);
	int a = rand() % 2;
	float x;
	if (a == 1) 
		x = -190;
	if (a == 0)
		x = 190;

	float y = std::max(50.0f, std::min(d(gen), 80.0f));
	return { x,y };
}

void RBGenerator::generateDisks(std::vector<Body*>& bodies, float t, float h) {
	if (t>=35 && t<45) {
		n = rate * h;
		f = f + (rate * h - n);
		if (f > 1) {
			n = n + 1;
			f = f - 1;
		}
		for (int i = 0; i < n; i++) {
			if (numOfRB <= max - 1) {
				Body* b = new Body(Disk(8), 0, 10, 30);
				b->restitution = 1.0;
				b->friction = 0.1;
				b->color = 0xFF8DA3CE;
				bodies.push_back(b);
				std::cout << "pushed\n";
				b->position = randomPos();
				numOfRB++;
			}

		}
	}
}
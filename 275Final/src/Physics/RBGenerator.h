#pragma once
#include "Body.h"
#include <vector>
#include <random>
#include <iostream>
class RBGenerator {
public:
	std::vector<Body*> bodies;
	float rate;
	float n;
	float f;
	Vec2 location;

	int numOfRB = 0;
	int max;

	RBGenerator();
	Vec2 randomPos();
	void generateDisks(std::vector<Body*>& particles, float t, float h);
};
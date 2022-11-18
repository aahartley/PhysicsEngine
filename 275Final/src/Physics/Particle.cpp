/*
Minor modifications were made in fall 2022, based on the original source code from Gustavo Pezzi, co-founder of Pikuma.com
*/

#include "Particle.h"
#include <iostream>
Particle::Particle(float x, float y, float mass) {
    this->position = Vec2(x, y);
    this->mass = mass;
    if (mass != 0.0) {
        this->invMass = 1.0 / mass;
    } else {
        this->invMass = 0.0;
    }
    //std::cout << "Particle constructor 1 called!" << std::endl;
}

Particle::Particle(float x, float y, float mass, float radius) {
	this->position = Vec2(x, y);
	this->mass = mass;
	this->radius = radius;
	this->color = 0xFF555555;
	this->initPos = Vec2(x, y);
	if (mass != 0.0) {
		this->invMass = 1.0 / mass;
	}
	else {
		this->invMass = 0.0;
	}

	//std::cout << "Particle constructor 2 called!" << std::endl;
}

Particle::~Particle() {
   // std::cout << "Particle destructor called!" << std::endl;
}

void Particle::addForce(const Vec2& force) {
    sumForces += force;
}

void Particle::clearForces() {
    sumForces = Vec2(0.0, 0.0);
}

void Particle::integrate(float dt) {
    // Find the acceleration based on the forces that are being applied and the mass
	acceleration = sumForces * invMass;

    // Integrate the acceleration to find the new velocity
    velocity += acceleration * dt;

    // Integrate the velocity to find the acceleration
    position += velocity * dt;

    clearForces();
}

void Particle::integrate(float dt, Vec2 initAccelaration, int vOP) {
	// Find the acceleration based on the forces that are being applied and the mass
	//acceleration = sumForces * invMass + initAccelaration;
	oldAcc = acceleration;
	acceleration = (sumForces * invMass) + initAccelaration;


	oldVel = velocity;
	// Integrate the acceleration to find the new velocity
	velocity += acceleration * dt;
	oldPos = position;
	if(vOP==1)
		vOperatorToSpot(dt);
	else if (vOP == 2)
		vOperatorIntro(dt);
	else if (vOP == 3)
		vOperatorOutro(dt);
	//std::cout << velocity.x << " " << velocity.y << '\n';
	// Integrate the velocity to find the acceleration
	position += ((velocity+oldVel)/2) * dt;   //added ave vel
	//std::cout << position.x << " " << position.y << '\n';
	if (position.x == endPos.x) {
		std::cout << "true\n";
	}
	clearForces();
}

void Particle::vOperatorToSpot(float dt) {
	Vec2 distance = endPos -position;
	
	velocity += distance*20*dt;
	velocity -= oldVel;
}
void Particle::vOperatorIntro(float dt) {
	std::random_device rd;
	std::mt19937 gen(rd());


	// instance of class std::normal_distribution with specific mean and stddev
	std::normal_distribution<float> d(-1, 2);
	float x = std::max(-5.0f, std::min(d(gen), 1.0f));

	float y = std::max(-5.0f, std::min(d(gen), 1.0f));
	velocity.x += x;
	velocity.y += y;
}
void Particle::vOperatorOutro(float dt) {
	int a = rand() % 2;
	if (a == 1) {
		Vec2 distance = Vec2{ -10,-200 } - position;

		velocity += distance * dt / 2;
	}
	else {
		Vec2 distance = Vec2{ 100,-200 } - position;

		velocity += -distance * dt / 2;
	}
	/*std::random_device rd;
	std::mt19937 gen(rd());


	 instance of class std::normal_distribution with specific mean and stddev
	std::normal_distribution<float> d(-1, 2);
	float x = std::max(-5.0f, std::min(d(gen), 1.0f));

	float y = std::max(-5.0f, std::min(d(gen), 1.0f));
	velocity.x += x;
	velocity.y += y;*/
}
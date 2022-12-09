/*
The source code is from Gustavo Pezzi, co-founder of Pikuma.com
*/

#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vec2.h"
#include <vector>
#include <random>
enum ParticleType {
    FIREWORK
};

struct Particle {
    //int radius;
    ParticleType particleType;
	float radius;
    
    Vec2 position;
    Vec2 oldPos;
    Vec2 initPos;
    Vec2 oldVel;
    Vec2 velocity;
    Vec2 oldAcc;
    Vec2 acceleration;

    Vec2 sumForces;

    Vec2 endPos;

    float mass;
    float invMass;
    bool alive = true;
    bool exploded = false;
    double fh;
    int color;

    Particle(float x, float y, float mass);
	Particle(float x, float y, float mass, float radius);
    ~Particle();

    void addForce(const Vec2& force);
    void clearForces();

    void integrate(float dt);
	void integrate(float dt, Vec2 initAccelaration, int vOP);	// the initial accelaration such as the gravity is involved

    void vOperatorIntro(float dt);
    void vOperatorToSpot(float dt);
    void vOperatorOutro(float dt);


};

#endif

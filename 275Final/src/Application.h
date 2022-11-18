/*
The source code is from Gustavo Pezzi, co-founder of Pikuma.com
*/

#ifndef APPLICATION_H
#define APPLICATION_H

#include "./Graphics.h"
#include "./Physics/Particle.h"
#include "./Physics/ParticleGenerator.h"
#include "./Physics/Body.h"
#include "./Physics/Contact.h"
#include "./Physics/CollisionDetection.h"
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>

class Application {
    private:
        bool running = false;
        std::vector<Particle*> particles;
        std::vector<Particle*> collidingP;
        std::vector<ParticleGenerator*> generators;
        std::vector<Body*> bodies;

 
        Vec2 pushForce = Vec2(0, 0);
        Vec2 mouseCursor = Vec2(0, 0);
        bool leftMouseButtonDown = false;

    public:
        Application() = default;
        ~Application() = default;
        bool isRunning();
        void setup();
        void input();
        void update();
        Particle* checkBoundaryCollisions();
        void boundaryCollisionResponse(Particle* p);
        void checkBoundary(Particle* p, float dt);
        void update(double deltaTime, double t);
        void render(float dt);
        void destroy();
};

#endif

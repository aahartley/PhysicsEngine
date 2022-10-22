/*
The source code is from Gustavo Pezzi, co-founder of Pikuma.com
*/

#ifndef APPLICATION_H
#define APPLICATION_H

#include "./Graphics.h"
#include "./Physics/Particle.h"
#include "./Physics/ParticleGenerator.h"
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
        void render();
        void destroy();
};

#endif

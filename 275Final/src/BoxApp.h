#ifndef APPLICATION_H
#define APPLICATION_H

#include "./Graphics.h"
#include "./Physics/Body.h"
#include <vector>


class Application {
    private:
        bool running = false;
		Vec2 pushForce = Vec2(0, 0);	// a constant force added for a time period
        std::vector<Body*> bodies;

    public:
        Application() = default;
        ~Application() = default;
        bool isRunning();
        void setup();
        void input();
        void update();
        void render();
        void destroy();
};

#endif

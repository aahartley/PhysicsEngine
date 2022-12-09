#include "BoxApp.h"
#include "./Physics/Constants.h"
#include "./Physics/Force.h"
#include "./Physics/CollisionDetection.h"
#include "./Physics/Contact.h"
#include <iostream>

bool Application::isRunning() {
    return running;
}

//====================================================================================//
// setup - open a window, initialize objects, and set up coordinate system origin     //
//====================================================================================//
void Application::setup() {
	running = Graphics::openWindow("colliding boxes");//openWindow();

	//------- set up the position of the coordinate system origin in the window -------
	Graphics::setOrigin(Graphics::windowWidth / 2, Graphics::windowHeight / 2);

	// Add static objects 
	Body* floor = new Body(Box(Graphics::getWindowWidth() * 1.0 / Graphics::unitLength, 1), 
		0, Graphics::getWindowHeight() *-0.5 / Graphics::unitLength + 0.5, -1.0);
	floor->restitution = 0.4;
	floor->color = 0xFF003366;
	bodies.push_back(floor);

	Body* ceiling = new Body(Box(Graphics::getWindowWidth() * 1.0 / Graphics::unitLength, 1),
		0, Graphics::getWindowHeight() * 0.5 / Graphics::unitLength - 0.5, -1.0);
	ceiling->restitution = 0.4;
	ceiling->color = 0xFF003366;
	bodies.push_back(ceiling);

	Body* left = new Body(Box(1, Graphics::getWindowHeight() * 1.0 / Graphics::unitLength - 2),
		Graphics::getWindowWidth() * -0.5 / Graphics::unitLength + 0.5, 0, -1.0);
	left->restitution = 0.4;
	left->color = 0xFF003366;
	bodies.push_back(left);

	Body* right = new Body(Box(1, Graphics::getWindowHeight() * 1.0 / Graphics::unitLength - 2),
		Graphics::getWindowWidth() * 0.5 / Graphics::unitLength - 0.5, 0, -1.0);
	right->restitution = 0.4;
	right->color = 0xFF003366;
	bodies.push_back(right);

	// Add a static box so other boxes can collide
	Body* bigBox = new Body(Box(8, 8), 0, 0, -1.0);
	bigBox->rotation = 3.14159 / 6;
	bigBox->restitution = 0.7;
	bigBox->color = 0xFF404040;
	bodies.push_back(bigBox);
}

//====================================================================================//
// input - handle the events                                                          //
//====================================================================================//
void Application::input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = false;
				if (event.key.keysym.sym == SDLK_UP)
					pushForce.y = 50;
                if (event.key.keysym.sym == SDLK_RIGHT)
                    pushForce.x = 50;
				if (event.key.keysym.sym == SDLK_DOWN)
					pushForce.y = -50;
                if (event.key.keysym.sym == SDLK_LEFT)
                    pushForce.x = -50;
                break;
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_UP)
                    pushForce.y = 0;
                if (event.key.keysym.sym == SDLK_RIGHT)
                    pushForce.x = 0;
                if (event.key.keysym.sym == SDLK_DOWN)
                    pushForce.y = 0;
                if (event.key.keysym.sym == SDLK_LEFT)
                    pushForce.x = 0;
                break;
            case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_RIGHT) {
					static int x, y;
					SDL_GetMouseState(&x, &y);
					Body* aBody = new Body(Box(2, 2), 
						Graphics::xPosInCoordinate(x), 
						Graphics::yPosInCoordinate(y), 10);
					//aBody->restitution = 0.75;
					aBody->velocity = { 10, 0 };
					aBody->color = 0xFF00FFFF;
					bodies.push_back(aBody);
				}
                break;
        }
    }
}

//====================================================================================//
// update - update the properties of the objects                                      //
//====================================================================================//
void Application::update() {
	//--------- Wait some time until reaching the target frame time in milliseconds -----
	static int timePreviousFrame;
	int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - timePreviousFrame);
	if (timeToWait > 0)
		SDL_Delay(timeToWait);

	//---------------Calculate the deltatime in seconds ---------------------------
	float deltaTime = (SDL_GetTicks() - timePreviousFrame) / 1000.0f;
	if (deltaTime > 0.016)
		deltaTime = 0.016;

	timePreviousFrame = SDL_GetTicks();	// Set the time of the current frame to be used in the next one

	//-------------------------- Apply forces to the disks ----------------------------
	for (auto body : bodies) {
		body->addForce(pushForce);		// added when an arrow key is pressed
	}

	//------------- update the body ---------------
	for (auto body : bodies) {
		body->update(deltaTime);
	}

	// Check all the rigidbodies with all other rigidbodies for collisions
	for (int i = 0; i < bodies.size() - 1; i++) {
		for (int j = i + 1; j < bodies.size(); j++) {
			Body* a = bodies[i];
			Body* b = bodies[j];

			Contact contact;

			if (CollisionDetection::isColliding(a, b, contact)) {
				// Resolve the collision using the impulse method
				contact.resolveCollisionWithRotation();
			}
		}
	}
}


//====================================================================================//
// render - render the objects for a frame                                            //
//====================================================================================//
void Application::render() {
	Graphics::clearScreen(0xFF21070F);	// clear the background
	Graphics::coordinateGrid(Graphics::unitLength, 0xFF222222);	// draw the coordinate grid for reference

	int color;
	// Draw all bodies
	for (auto body : bodies) {
		color = body->color;
		if (body->shape->getType() == DISK) {
			Disk* disk = (Disk*)body->shape;
			Graphics::drawDisk(body->position.x, body->position.y,
				disk->radius, body->rotation, color);
		}

		if (body->shape->getType() == BOX) {
			Box* box = (Box*)body->shape;
			Graphics::drawFilledPolygon(box->worldVertices, color);
		}
	}

	Graphics::renderFrame();
}

//====================================================================================//
// desdroy - release the resources                                                    //
//====================================================================================//
void Application::destroy() {
    for (auto body: bodies) {
        delete body;
    }
    Graphics::CloseWindow();
}

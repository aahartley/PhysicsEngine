#include "./BoxApp.h"
#include "./Physics/Constants.h"
#include "./Physics/Force.h"
#include "./Physics/CollisionDetection.h"
#include "./Physics/Contact.h"
#include <iostream>		// for cout ...


bool Application::isRunning() {
    return running;
}

//====================================================================================//
// setup - open a window, initialize objects, and set up coordinate system origin     //
//====================================================================================//
void Application::setup() {
	running = Graphics::openWindow("pegboard simulation");	//open a window for rendering

	//------- set up the position of the coordinate system origin in the window -------
	Graphics::setOrigin(Graphics::windowWidth / 2, Graphics::windowHeight / 2);

    //-------------------------- Add static objects -----------------------------------
    Body* floor = new Body(Box(Graphics::getWindowWidth() * 1.0 / Graphics::unitLength,  1), 
		0, Graphics::getWindowHeight() * -0.5 / Graphics::unitLength + 0.5, -1);
    floor->restitution = 0.0;
	floor->friction = 0.7;
	floor->color = 0xFF003366;
    bodies.push_back(floor);

	Body* ceiling = new Body(Box(Graphics::getWindowWidth() * 1.0 / Graphics::unitLength, 1),
		0, Graphics::getWindowHeight() * 0.5 / Graphics::unitLength - 0.5, -1);
	ceiling->restitution = 0.2;
	ceiling->friction = 0.7;
	ceiling->color = 0xFF003366;
	bodies.push_back(ceiling);

	Body* left = new Body(Box(1, Graphics::getWindowHeight() * 1.0 / Graphics::unitLength -2),
		Graphics::getWindowWidth() * -0.5 / Graphics::unitLength + 0.5, 0, -1);
	left->restitution = 0.2;
	left->friction = 0.7;
	left->color = 0xFF003366;
	bodies.push_back(left);

	Body* right = new Body(Box(1, Graphics::getWindowHeight() * 1.0 / Graphics::unitLength -2),
		Graphics::getWindowWidth() * 0.5 / Graphics::unitLength - 0.5, 0, -1);
	right->restitution = 0.2;
	right->friction = 0.7;
	right->color = 0xFF003366;
	bodies.push_back(right);

    //------------------ funnel --------------------
	Body* leftTopBoard = new Body(Box(8, 1), -4.25, Graphics::yPosInCoordinate(Graphics::windowHeight/6)+1.5, -1);
	leftTopBoard->rotation = -3.14159 / 4;
	leftTopBoard->restitution = 0.5;
	leftTopBoard->friction = 0.5;
	leftTopBoard->color = 0xFF003366;
	bodies.push_back(leftTopBoard);

	Body* leftBead = new Body(Disk(0.5), -1., Graphics::yPosInCoordinate(Graphics::windowHeight / 6)-2, -1);
	leftBead->restitution = 0.5;
	leftBead->friction = 0.5;
	leftBead->color = 0xFF003366;
	bodies.push_back(leftBead);

	Body* leftBottomBoard = new Body(Box(1, 2), -1., Graphics::yPosInCoordinate(Graphics::windowHeight / 6)-3.25, -1);
	leftBottomBoard->restitution = 0.5;
	leftBottomBoard->friction = 0.5;
	leftBottomBoard->color = 0xFF003366;
	bodies.push_back(leftBottomBoard);

	Body* rightTopBoard = new Body(Box(8, 1), 4.25, Graphics::yPosInCoordinate(Graphics::windowHeight / 6)+1.5, -1);
	rightTopBoard->rotation = 3.14159 / 4;
	rightTopBoard->restitution = 0.5;
	rightTopBoard->friction = 0.5;
	rightTopBoard->color = 0xFF003366;
	bodies.push_back(rightTopBoard);

	Body* rightBead = new Body(Disk(0.5), 1., Graphics::yPosInCoordinate(Graphics::windowHeight / 6) - 2, -1);
	rightBead->restitution = 0.5;
	rightBead->friction = 0.5;
	rightBead->color = 0xFF003366;
	bodies.push_back(rightBead);

	Body* rightBottomBoard = new Body(Box(1, 2), 1., Graphics::yPosInCoordinate(Graphics::windowHeight / 6)-3.25, -1);
	rightBottomBoard->restitution = 0.5;
	rightBottomBoard->friction = 0.5;
	rightBottomBoard->color = 0xFF003366;
	bodies.push_back(rightBottomBoard);

	//-------------------------- pegs ----------------------------
	for (int i = 0; i < 7; i++) {
		for (int j = -i-16; j <= i+16; j=j+2) {
			Body* peg = new Body(Disk(0.5), j*1., (2-i)*2.5, -1);
			//peg->restitution = 1;
			//peg->friction = 0.75;
			peg->color = 0xFF003366;
			bodies.push_back(peg);
		}
	}

	//-------------- buckets --------------------------
	for (int i = 0; i < 25; i++) {
			Body* separator = new Body(Box(0.2, 2), (12-i)*2, Graphics::yPosInCoordinate(Graphics::windowHeight)+1, -1);
			separator->restitution = 0.2;
			separator->friction = 0.0;
			separator->color = 0xFF003366;
			bodies.push_back(separator);
	}
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
				if (event.key.keysym.sym == SDLK_ESCAPE)
					running = false;
				if (event.key.keysym.sym == SDLK_UP)
					pushForce.y = 100;
				if (event.key.keysym.sym == SDLK_RIGHT)
					pushForce.x = 100;
				if (event.key.keysym.sym == SDLK_DOWN)
					pushForce.y = -100;
				if (event.key.keysym.sym == SDLK_LEFT)
					pushForce.x = -100;
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
                int x, y;
                SDL_GetMouseState(&x, &y);
				if (event.button.button == SDL_BUTTON_LEFT) {
					for (int i = 0; i < 5; i++) {
						for (int j = 0; j < 5; j++) {
							Body* disk = new Body(Disk(0.25),
								(2-i)*1.0,
								Graphics::yPosInCoordinate(0) -1.5 - j*1.0, 1);
							disk->restitution = 0.65;
							disk->friction = 0.0;
							disk->color = 0xFF00FFFF;
							bodies.push_back(disk);
						}
					}
				}
				else if (event.button.button == SDL_BUTTON_RIGHT) {
					;
				}
                break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update objects)
///////////////////////////////////////////////////////////////////////////////
void Application::update() {
    // Wait some time until the reach the target frame time in milliseconds
    static int timePreviousFrame;
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - timePreviousFrame);
    if (timeToWait > 0)
        SDL_Delay(timeToWait);

    // Calculate the deltatime in seconds
    float deltaTime = (SDL_GetTicks() - timePreviousFrame) / 1000.0f;
    if (deltaTime > 0.016)
        deltaTime = 0.016;

    // Set the time of the current frame to be used in the next one
    timePreviousFrame = SDL_GetTicks();

    // Apply forces to the bodies
    for (auto body: bodies) {
		body->addForce(pushForce);		// added when an arrow key is pressed

        // Apply the weight force
		Vec2 weight = Vec2(0.0, body->mass * -9.8*4); // scaled properly
		body->addForce(weight);
    }

    // Integrate the acceleration and velocity to estimate the new position
    for (auto body: bodies) {
        body->update(deltaTime);
    }

    // Check all the rigidbodies with the other rigidbodies for collision
    for (int i = 0; i <= bodies.size() - 1; i++) {
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

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::render() {
	Graphics::clearScreen(0xFF21070F);	// clear the background
	Graphics::coordinateGrid(Graphics::unitLength, 0xFF222222);	// draw the coordinate grid for reference


    //-------- render boxes with textures ------------
	//int color;
	for (int i = 0; i < bodies.size(); i++) {
		Body *body = bodies[i];

		if (i < 4) {	// draw the floor, ceiling, left and right sides
	//		color = body->color;
			if (body->shape->getType() == BOX) {
				Box* box = (Box*)body->shape;
				Graphics::drawFilledPolygon(box->worldVertices, body->color);
			}
		}
		else {			// for other objects
			if (body->shape->getType() == BOX) {
				Box* box = (Box*)body->shape;
				Graphics::drawFilledPolygon(box->worldVertices, body->color);
			}
			else if (body->shape->getType() == DISK) {
				Disk* disk = (Disk*)body->shape;
				if(body->isStatic())
					Graphics::drawDisk(body->position.x, body->position.y,
						disk->radius, body->color);
				else
					Graphics::drawCircle(body->position.x, body->position.y,
					disk->radius, body->color);
			}
		}
	}

    Graphics::renderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::destroy() {
    for (auto body: bodies) {
        delete body;
    }

	Graphics::CloseWindow();
}

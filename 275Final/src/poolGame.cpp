//#include "./BoxApp.h"
#include "./DiskApp.h"
#include "./Physics/Constants.h"
#include "./Physics/Force.h"
#include "./Physics/CollisionDetection.h"
#include "./Physics/Contact.h"
#include <iostream>		// for cout ...

float r = 1.5;	// radius of a disk
float strikePower = 50;	
float diskRestitution = 0.9;
float diskFriction = 0.1;
float trapSpeed = 50;
float cueDiskPosX;
float target1PosX;
float boundaryRestitution = 0.5;
float boundaryFriction = 0.75;

SDL_Surface* image = NULL;
SDL_Texture* textures[10];	// could be 15

//------------- measures of the boundaries --------------
float width, height, thickness = 2;

//----------- positions of the pockets ----------------
float topRightX, topRightY;
Vec2 topRightPos;
float topMiddleX, topMiddleY;
Vec2 topMiddlePos;
//TODO: positions of other pockets
//------------------------------------------------------

bool Application::isRunning() {
    return running;
}

//====================================================================================//
// setup - open a window, initialize objects, and set up coordinate system origin     //
//====================================================================================//
void Application::setup() {
//    running = Graphics::openWindow();
    
	running = Graphics::openWindow("pool game");	//open a window for rendering

	//------- set up the position of the coordinate system origin in the window -------
	Graphics::setOrigin(Graphics::windowWidth / 2, Graphics::windowHeight / 2);

	//----------------- create the textures -----------------
	image = IMG_Load("./assets/number1.png");
	if (image) {
		textures[0] = SDL_CreateTextureFromSurface(Graphics::renderer, image);
		std::cout << "crate texture created" << std::endl;
		SDL_FreeSurface(image);
	}

	image = IMG_Load("./assets/number2.png");
	if (image) {
		textures[1] = SDL_CreateTextureFromSurface(Graphics::renderer, image);
		std::cout << "crate texture created" << std::endl;
		SDL_FreeSurface(image);
	}

	image = IMG_Load("./assets/number3.png");
	if (image) {
		textures[2] = SDL_CreateTextureFromSurface(Graphics::renderer, image);
		std::cout << "crate texture created" << std::endl;
		SDL_FreeSurface(image);
	}

	image = IMG_Load("./assets/number4.png");
	if (image) {
		textures[3] = SDL_CreateTextureFromSurface(Graphics::renderer, image);
		std::cout << "crate texture created" << std::endl;
		SDL_FreeSurface(image);
	}
	//TODO: create other textures

	//----------- the measures of the screen in math coordinates ---------------
	width = Graphics::getWindowWidth() * 1.0 / Graphics::unitLength;	
	height = Graphics::getWindowHeight() * 1.0 / Graphics::unitLength;

	//----------- cue disk position and target 1 position ------------------
	cueDiskPosX = (width -2*thickness) * -0.35;
	target1PosX = (width - 2 * thickness) * 0.25;

	//------------------ initialize the positions of the pockets -----------------
	topRightX = width/2 - thickness;
	topRightY = height/2 - thickness;
	topRightPos = Vec2(topRightX, topRightY);

	topMiddleX = 0;
	topMiddleY = height / 2 - thickness;
	topMiddlePos = Vec2(topMiddleX, topMiddleY);
	//TODO: positions of other pockets
	//----------------------------------------------------------------------------
 
	//-------------------------- Add boundaries -----------------------------------
    Body* floor = new Body(Box(width, thickness), 0, (height-thickness)*-0.5, -1);
    floor->restitution = boundaryRestitution;
	floor->friction = boundaryFriction;
	floor->color = 0xFF003366;
    bodies.push_back(floor);

	Body* ceiling = new Body(Box(width, thickness), 0, (height - thickness)*0.5, -1);
	ceiling->restitution = boundaryRestitution;
	ceiling->friction = boundaryFriction;;
	ceiling->color = 0xFF003366;
	bodies.push_back(ceiling);
	
	Body* left = new Body(Box(thickness, height - 2 * thickness), (width - thickness)*-0.5, 0, -1);
	left->restitution = boundaryRestitution;
	left->friction = boundaryFriction;;
	left->color = 0xFF003366;
	bodies.push_back(left);

	Body* right = new Body(Box(thickness, height - 2 * thickness), (width - thickness)*0.5, 0, -1);
	right->restitution = boundaryRestitution;
	right->friction = boundaryFriction;;
	right->color = 0xFF003366;
	bodies.push_back(right);
	//-----------------------------------------------------------------------

	//----------- set up the disks -----------------
	Body* cueDisk = new Body(Disk(r), cueDiskPosX, 0, 10);
	cueDisk->restitution = diskRestitution;
	cueDisk->friction = diskFriction;
	cueDisk->color = 0xFFFFFFFF;
	cueDisk->identifier = 0;
	bodies.push_back(cueDisk);

	Body* target1 = new Body(Disk(r), target1PosX, 0, 10);
	target1->restitution = diskRestitution;
	target1->friction = diskFriction;
	target1->color = 0xFF00FFFF;
	target1->identifier = 1;
	bodies.push_back(target1);

	Body* target2 = new Body(Disk(r), target1PosX + r * sqrt(3), r, 10);
	target2->restitution = diskRestitution;
	target2->friction = diskFriction;
	target2->color = 0xFF00FF00;
	target2->identifier = 2;
	bodies.push_back(target2);

	Body* target3 = new Body(Disk(r), target1PosX + r * sqrt(3), -r, 10);
	target3->restitution = diskRestitution;
	target3->friction = diskFriction;
	target3->color = 0xFFFF00FF;
	target3->identifier = 3;
	bodies.push_back(target3);

	Body* target4 = new Body(Disk(r), target1PosX + r * 2*sqrt(3), 0, 10);
	target4->restitution = diskRestitution;
	target4->friction = diskFriction;
	target4->color = 0xFFFFFF00;
	target4->identifier = 4;
	bodies.push_back(target4);
	//TODO: more target disks
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
				break;
			case SDL_MOUSEMOTION:
				mouseCursor.x = event.motion.x;
				mouseCursor.y = event.motion.y;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (!leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT) {
					leftMouseButtonDown = true;
					int x, y;
					SDL_GetMouseState(&x, &y);
					mouseCursor.x = x;
					mouseCursor.y = y;
				} //TODO: use the right button and the middle button to adjust the strike power
				break;
			case SDL_MOUSEBUTTONUP:
				if (leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT && bodies.size() > 4) {
					if (bodies[4]->identifier == 0) {
						leftMouseButtonDown = false;
						bodies[4]->velocity = (bodies[4]->position - Graphics::windowToCoordinate(mouseCursor)).normalize() * strikePower;	// simply set up the new velocity
					}
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
		Vec2 friction = Force::generateFrictionForce(*body, 75);
		if (body->velocity.magnitude() > 0.01)
			body->addForce(friction);

		body->rotation *=0.995;
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

	for (int i = 4; i < bodies.size(); i++) {
		Body *body = bodies[i];
		if (((body->position - topRightPos).magnitude() < 2 * r
			|| (body->position - topMiddlePos).magnitude() < 2 * r	//TODO: distances to other pockets
			)
			&& body->velocity.magnitude() < trapSpeed) {

			bodies.erase(bodies.begin() + i);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::render() {
	Graphics::clearScreen(0xFF004400);	// clear the background
	//Graphics::coordinateGrid(Graphics::unitLength, 0xFF222222);	// draw the coordinate grid for reference

	//--------------------- draw pockets ------------------------
	Graphics::drawDisk(topRightX, topRightY, 2.5*r, 0xFF000000);
	Graphics::drawDisk(topMiddleX, topMiddleY, 2*r, 0xFF000000);
	//TODO: other pockets

    //-------- render objects with textures ------------
	int color;
	for (int i = 0; i < bodies.size(); i++) {
		Body *body = bodies[i];

		if (i < 4) {	// draw the floor, ceiling, left and right sides
			color = body->color;
			if (body->shape->getType() == BOX) {
				Box* box = (Box*)body->shape;
				Graphics::drawFilledPolygon(box->worldVertices, color);
			}
		}
		else {			// for other objects
			if (body->shape->getType() == DISK) {
				Disk* disk = (Disk*)body->shape;
				Graphics::drawDisk(body->position.x, body->position.y, disk->radius, body->color);

				SDL_Rect dstrect = { Graphics::xPosInWindow(body->position.x - disk->radius / 2),
					Graphics::yPosInWindow(body->position.y + disk->radius / 2),
					Graphics::unitLength * disk->radius * 1, Graphics::unitLength * disk->radius * 1 };
				SDL_RenderCopyEx(Graphics::renderer, textures[body->identifier - 1], NULL, &dstrect, body->rotation*-57.295779513, NULL, SDL_FLIP_NONE);
			}
		}
	}

	//--- draw a line connecting the mouse postion and the cue disk when the mouse if left pressed -----
	if (leftMouseButtonDown && bodies.size() > 4) {
		if (bodies[4]->identifier == 0) {
			Graphics::drawLine(bodies[4]->position.x, bodies[4]->position.y,
				Graphics::xPosInCoordinate(mouseCursor.x), Graphics::yPosInCoordinate(mouseCursor.y), 0xFFFFFFFF);
		}
	}

    Graphics::renderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::destroy() {
	for (auto body : bodies) {
		delete body;
	}

	for (auto t : textures)
		SDL_DestroyTexture(t);

	Graphics::CloseWindow();
}

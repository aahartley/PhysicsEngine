#include "./BoxApp.h"
#include "./Physics/Constants.h"
#include "./Physics/Force.h"
#include "./Physics/CollisionDetection.h"
#include "./Physics/Contact.h"
#include <iostream>		// for cout ...


SDL_Surface* image = NULL;
SDL_Texture* textureCrate = NULL;
SDL_Texture* textureBasketball = NULL;

bool Application::isRunning() {
    return running;
}

//====================================================================================//
// setup - open a window, initialize objects, and set up coordinate system origin     //
//====================================================================================//
void Application::setup() {
//    running = Graphics::openWindow();
    
	running = Graphics::openWindow("colliding disks and boxes");	//open a window for rendering

	//------- set up the position of the coordinate system origin in the window -------
	Graphics::setOrigin(Graphics::windowWidth / 2, Graphics::windowHeight / 2);

	//-------- create the texture from an image
	image = IMG_Load("./assets/crate.png");
	if (image) {
		textureCrate = SDL_CreateTextureFromSurface(Graphics::renderer, image);
		std::cout << "crate texture created" << std::endl;
		SDL_FreeSurface(image);
	}

	image = IMG_Load("./assets/basketball.png");
	if (image) {
		textureBasketball = SDL_CreateTextureFromSurface(Graphics::renderer, image);
		std::cout << "basket ball texture created" << std::endl;
		SDL_FreeSurface(image);
	}

    //-------------------------- Add boundaries -----------------------------------
    Body* floor = new Body(Box(Graphics::getWindowWidth() * 1.0 / Graphics::unitLength,  1), 
		0, Graphics::getWindowHeight() * -0.5 / Graphics::unitLength + 0.5, -1);
    floor->restitution = 0.3;
	floor->friction = 0.7;
	floor->color = 0xFF003366;
    bodies.push_back(floor);

	Body* ceiling = new Body(Box(Graphics::getWindowWidth() * 1.0 / Graphics::unitLength, 1),
		0, Graphics::getWindowHeight() * 0.5 / Graphics::unitLength - 0.5, -1);
	ceiling->restitution = 0.3;
	ceiling->friction = 0.7;
	ceiling->color = 0xFF003366;
	bodies.push_back(ceiling);

	Body* left = new Body(Box(1, Graphics::getWindowHeight() * 1.0 / Graphics::unitLength -2),
		Graphics::getWindowWidth() * -0.5 / Graphics::unitLength + 0.5, 0, -1);
	left->restitution = 0.3;
	left->friction = 0.7;
	left->color = 0xFF003366;
	bodies.push_back(left);

	Body* right = new Body(Box(1, Graphics::getWindowHeight() * 1.0 / Graphics::unitLength -2),
		Graphics::getWindowWidth() * 0.5 / Graphics::unitLength - 0.5, 0, -1);
	right->restitution = 0.3;
	right->friction = 0.7;
	right->color = 0xFF003366;
	bodies.push_back(right);

    //------------------ Add a static box so other boxes can collide --------------------
    Body* bigBox = new Body(Box(8, 8), 0, 0, -1);
    bigBox->rotation = -3.14159 / 6;
    bigBox->restitution = 0.75;
	bigBox->friction = 0.5;
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
					Body* box = new Body(Box(4, 4), Graphics::xPosInCoordinate(x),
						Graphics::yPosInCoordinate(y), 10.0);
					box->restitution = 0.4;
					box->friction = 0.5;
					bodies.push_back(box);
				}
				else if (event.button.button == SDL_BUTTON_RIGHT) {
					Body* disk = new Body(Disk(2),
						Graphics::xPosInCoordinate(x),
						Graphics::yPosInCoordinate(y), 1);
					disk->restitution = 0.9;
					disk->friction = 0.4;
					bodies.push_back(disk);
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
		Vec2 weight = Vec2(0.0, body->mass * -9.8 * 2); // scaled properly
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
			if (body->shape->getType() == BOX) {
				Box* box = (Box*)body->shape;
				SDL_Rect dstrect = { Graphics::xPosInWindow(body->position.x - box->width * 0.5),
					Graphics::yPosInWindow(body->position.y + box->height * 0.5),
					Graphics::unitLength * box->width, Graphics::unitLength * box->height };
				SDL_RenderCopyEx(Graphics::renderer, textureCrate, NULL, &dstrect, body->rotation*-57.295779513, NULL, SDL_FLIP_NONE);
			}
			else if (body->shape->getType() == DISK) {
				Disk* disk = (Disk*)body->shape;
				SDL_Rect dstrect = { Graphics::xPosInWindow(body->position.x - disk->radius),
					Graphics::yPosInWindow(body->position.y + disk->radius),
					Graphics::unitLength * disk->radius * 2, Graphics::unitLength * disk->radius * 2 };
				SDL_RenderCopyEx(Graphics::renderer, textureBasketball, NULL, &dstrect, body->rotation*-57.295779513, NULL, SDL_FLIP_NONE);	// display in a particular rectangle
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

	SDL_DestroyTexture(textureCrate);
	SDL_DestroyTexture(textureBasketball);
	Graphics::CloseWindow();
}

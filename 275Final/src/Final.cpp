#include "Application.h"
#include "./Physics/Constants.h"
#include "./Physics/Force.h"

#include <iostream>

SDL_Surface* image = NULL;
SDL_Texture* textures[5];
std::vector<Vec2> explosions;
std::vector<Particle*> eParticles;
int backgroundColor=0xFF22254A;
int diskColor=0xFF8DA3CE;
int squareColor=0xFF4B53A8;
bool Application::isRunning() {
	return running;
}

//====================================================================================//
// setup - open a window, initialize objects, and set up coordinate system origin     //
//====================================================================================//
void Application::setup() {
	running = Graphics::openWindow("Austin Hartley Final");//openWindow();

	//----------- set up the position of the coordinate system origin in the window -----------------
	Graphics::setOrigin(Graphics::windowWidth / 2, Graphics::windowHeight * .8);
	image = IMG_Load("./assets/wood2.jpg");
	if (image) {
		textures[0] = SDL_CreateTextureFromSurface(Graphics::renderer, image);
		//std::cout << "crate texture created" << std::endl;
		SDL_FreeSurface(image);
	}

	image = IMG_Load("./assets/squaredreamNEW.png");
	SDL_Surface* formattedImage = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_ARGB8888, 0);
	if (image) {
		textures[1] = SDL_CreateTextureFromSurface(Graphics::renderer, image);
		//std::cout << "basket ball texture created" << std::endl;
		SDL_FreeSurface(image);
	}
	//std::cout << "bytes "<<(int)formattedImage->format->BytesPerPixel<<'\n';
	int w = formattedImage->w;
	int h = formattedImage->h;
	ParticleGenerator* gen = new ParticleGenerator(Vec2{ -100,100 },1000);
	ParticleGenerator* gen2 = new ParticleGenerator(Vec2{ 100,100 },1000);
	ParticleGenerator* gen3 = new ParticleGenerator(Vec2{ 0,100 },1000);
	ParticleGenerator* gen4 = new ParticleGenerator(Vec2{ 0,100 },1000);
	ParticleGenerator* gen5 = new ParticleGenerator(Vec2{ 0,0 },1);

	RBGenerator* rbGen = new RBGenerator();
	generators.push_back(gen);
	generators.push_back(gen2);
	generators.push_back(gen3);
	generators.push_back(gen4);
	generators.push_back(gen5);

	rbGenerators.push_back(rbGen);

	//std::cout << w << " " << h<<'\n';
	Uint32* pixels = (Uint32*)formattedImage->pixels;

	for (float y = 0; y < h; y += 5) {
		for (float x = 0; x < w; x += 5) {
			int index = (y * w + x);
			Uint8 r, g, b, a;
			SDL_GetRGBA(pixels[index], formattedImage->format, &r, &g, &b, &a);
			if (a > 0) {
				//std::cout << index << '\n';
				//Graphics::xPosInCoordinate(x + 200)
				Particle* p = new Particle(Graphics::xPosInCoordinate(x + 350), Graphics::yPosInCoordinate(y + 200), 1, 0.3);
				p->color = (a << 24) + (b << 16) + (g << 8) + r;
				//particles.push_back(p);
				int color = (a << 24) + (b << 16) + (g << 8) + r;
				if (index < 900000) {
					gen->pos.push_back(Vec2{ Graphics::xPosInCoordinate(x +50),Graphics::yPosInCoordinate(y ) });
					gen->colors.push_back(color);
				}
				else {
					gen2->pos.push_back(Vec2{ Graphics::xPosInCoordinate(x+50 ),Graphics::yPosInCoordinate(y) });
					gen2->colors.push_back(color);
				}
			}
		}
	}

	image = IMG_Load("./assets/heart.png");
	formattedImage = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_ARGB8888, 0);
	if (image) {
		textures[2] = SDL_CreateTextureFromSurface(Graphics::renderer, image);
		//std::cout << "basket ball texture created" << std::endl;
		SDL_FreeSurface(image);
	}
	w = formattedImage->w;
	h = formattedImage->h;
	Uint32* pixels2 = (Uint32*)formattedImage->pixels;

	for (float y = 0; y < h; y += 5) {
		for (float x = 0; x < w; x += 5) {
			int index = (y * w + x);
			Uint8 r, g, b, a;
			SDL_GetRGBA(pixels2[index], formattedImage->format, &r, &g, &b, &a);
			if (a > 0) {
				//std::cout << index << '\n';
				//Graphics::xPosInCoordinate(x + 200)
				Particle* p = new Particle(Graphics::xPosInCoordinate(x + 750), Graphics::yPosInCoordinate(y + 200), 1, 0.3);
				p->color = (a << 24) + (b << 16) + (g << 8) + r;
				//particles.push_back(p);
				int color = (a << 24) + (b << 16) + (g << 8) + r;
				gen3->pos.push_back(Vec2{ Graphics::xPosInCoordinate(x + 850),Graphics::yPosInCoordinate(y + 400) });
				gen3->colors.push_back(color);
				
			}
		}
	}
	gen4->keepStatic = true;
	image = IMG_Load("./assets/text.png");
	formattedImage = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_ARGB8888, 0);
	if (image) {
		textures[3] = SDL_CreateTextureFromSurface(Graphics::renderer, image);
		//std::cout << "basket ball texture created" << std::endl;
		SDL_FreeSurface(image);
	}
	w = formattedImage->w;
	h = formattedImage->h;
	Uint32* pixels3 = (Uint32*)formattedImage->pixels;

	for (float y = 0; y < h; y += 5) {
		for (float x = 0; x < w; x += 5) {
			int index = (y * w + x);
			Uint8 r, g, b, a;
			SDL_GetRGBA(pixels3[index], formattedImage->format, &r, &g, &b, &a);
			if (a > 0) {
				//std::cout << index << '\n';
				//Graphics::xPosInCoordinate(x + 200)
				Particle* p = new Particle(Graphics::xPosInCoordinate(x ), Graphics::yPosInCoordinate(y ), 1, 0.3);
				p->color = (a << 24) + (b << 16) + (g << 8) + r;
				//particles.push_back(p);
				int color = (a << 24) + (b << 16) + (g << 8) + r;
				gen4->pos.push_back(Vec2{ Graphics::xPosInCoordinate(x +400),Graphics::yPosInCoordinate(y+350 ) });
				gen4->colors.push_back(color);

			}
		}
	}
	
	//std::cout << "pic made";
	//SDL_SetRenderTarget(Graphics::renderer, texture);
	//SDL_RenderReadPixels(Graphics::renderer, NULL, SDL_PIXELFORMAT_RGBA32, pixels, 2);

	//Particle* heavy = new Particle(-5, 25, 10, 2);
	//Particle* light = new Particle(0, 100, 1, 1);
	//light->velocity = Vec2{ 10,0 };
	//particles.push_back(light);
	//particles.push_back(heavy);


	//std::ifstream fin("my_file.txt");
	//for (std::string line; std::getline(fin, line); )   //read stream line by line
	//{
	//	std::istringstream in(line);      //make a stream for the line itself


	//	float x, y;
	//	in >> x >> y;       //now read the whitespace-separated floats
	//	Particle* readP = new Particle(x, y, 1, 1);
	//	particles.push_back(readP);
	//}
	Body* floor = new Body(Box(1000, 6),
		0, -3, -1);
	floor->restitution = 0.9;
	floor->friction = 0.0;
	floor->color = 0;
	bodies.push_back(floor);
	
	Body* rectangle = new Body(Box(15, 15), 0, 20, 500.0);
	rectangle->restitution = 0.9;
	rectangle->friction = 0.1;
	rectangle->color = squareColor;
	bodies.push_back(rectangle);
	
	Body* leftD = new Body(Disk(8), -190, 20, 30);
	leftD->restitution = 1.0;
	leftD->friction = 0.1;
	leftD->color = diskColor;
	bodies.push_back(leftD);

	Body* rightD = new Body(Disk(8), 190, 20, 30);
	rightD->restitution = 1.0;
	rightD->friction = 0.1;
	rightD->color = diskColor;
	bodies.push_back(rightD);



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
		case SDL_MOUSEMOTION:
			if (event.button.button == SDL_BUTTON_LEFT) {
				int x, y;
				SDL_GetMouseState(&x, &y);
				//Particle* aDot = new Particle(Graphics::xPosInCoordinate(x), Graphics::yPosInCoordinate(y), 1.0, 1);
				//float v_x, v_y;
				//v_x = (float)(rand() % 50 - 25);
				//v_y = (float)(rand() % 50 - 25);
				//aDot->velocity = Vec2(v_x, v_y);
				//particles.push_back(aDot);
			}
			break;
		}
	}
}
void Application::update() {};
//====================================================================================//
// update - update the properties of the objects                                      //
//====================================================================================//
void Application::update(double deltaTime, double t) {
	static int timePreviousFrame;
	int timeToWait = 46 - (SDL_GetTicks() - timePreviousFrame);
	if (timeToWait > 0)
		SDL_Delay(timeToWait);
	timePreviousFrame = SDL_GetTicks();


	double TimestepRemaining = deltaTime;
	double Timestep = TimestepRemaining;
	//while (TimestepRemaining > 0.002) {
		//std::cout << "remain: " << TimestepRemaining << '\n';

		//-------------------------- Apply forces to the particles ----------------------------
	for (auto p : particles) {
		//std::cout << p->position.x << '\n';

		//particle->addForce(pushForce);	// apply push force
	}
	for (int i = 0; i < generators.size();i++) {
		if(i<2)
			generators[i]->generateParticles(particles, t, Timestep, 0,10);
		else if(i==2)
			generators[i]->generateParticles(particles, t, Timestep, 68, 75);
		else if (i==3)
			generators[i]->generateParticles(particles, t, Timestep, 76, 80);
		else if(i==4)
			generators[i]->generateFireworks(particles, t, Timestep, 76, 83);



	}
	for (auto gen : rbGenerators) {
		gen->generateDisks(bodies, t, Timestep);

	}
	if (t > 25) {
		for (auto body : bodies) {
			//body->addTorque(0.1);
			body->addForce(Vec2(0, -10 * body->mass));
			if (body->shape->getType() == DISK && t<31) {
				body->addTorque(100);
				body->update(deltaTime, 1,200);
			}
			else if (body->shape->getType() == DISK && t >= 31 && t<35) {
				body->rotation = 0;
			
			}
			else if (body->shape->getType() == DISK && t >=35 && t<42) {
				body->update(deltaTime, 1,1000);
			}
			else if (t >= 42 && t < 43 &&body->shape->getType() == BOX && !body->isStatic()) {
				body->position = Vec2(2,-3);
			}
			else if (t >= 45 && t < 57 && body->shape->getType() == BOX && !body->isStatic()) {
				body->addForce(Vec2(0, -5000));
				body->addTorque(5000);
				body->update(deltaTime, 0, 0);

			}
			else {
				body->update(deltaTime, 0,0);
			}


		}
	}
	if (t > 25) {
		// Check all the rigidbodies with all other rigidbodies for collisions
		for (int i = 0; i < bodies.size() - 1; i++) {
			for (int j = i + 1; j < bodies.size(); j++) {
				Body* a = bodies[i];
				Body* b = bodies[j];

				Contact contact;

				if (CollisionDetection::isColliding(a, b, contact)) {
					//Resolve the collision using the impulse method
					contact.resolveCollisionWithRotation();
				}
			}
		}
	}
	

	//------------- Integrate the acceleration and velocity to estimate the new position ---------------
	for (int i = 0; i < particles.size();++i) {
		//implement: if particle needs to rest
		auto particle = particles[i];
		if (particle->alive) {
			if (t <= 3) {
				particle->integrate(Timestep, Vec2(0, 15), 2);
				Vec2 drag = Force::generateDragForce(*particle, 0.4);

				if (particle->velocity.magnitude() >= 0.0) {
					particle->addForce(drag);
					int a = rand() % 2;
					if (a == 1) {
						Vec2 wind = Force::generateWindForce(*particle, .7, 100);
						particle->addForce(wind);
					}
					if (a == 0) {
						Vec2 wind = Force::generateWindForce(*particle, -.7, 1);
						particle->addForce(wind);
					}
				}
			}
			//form pic
			else if (t > 3 && t <= 14) {
				particle->integrate(Timestep, Vec2(0, -10), 1);
				Vec2 wind = Force::generateWindForce(*particle, .7, 1);
				Vec2 drag = Force::generateDragForce(*particle, 0.4);

				if (particle->velocity.magnitude() >= 0.0) {
					particle->addForce(drag);
					int a = rand() % 2;
					if (a == 1)
						particle->addForce(wind);
				}

			}
			//outro
			else if (t > 14 && t <= 24) {
				particle->integrate(Timestep, Vec2(0, -10), 3);
				Vec2 wind = Force::generateWindForce(*particle, .7, 1);
				Vec2 drag = Force::generateDragForce(*particle, 0.4);

				if (particle->velocity.magnitude() >= 0.0) {
					particle->addForce(drag);
					int a = rand() % 2;
					if (a == 1)
						particle->addForce(wind);
				}

			}
			else if (t > 76 && t <= 90) {
				particle->integrate(Timestep, Vec2(0, -10), 0);
				//Vec2 wind = Force::generateWindForce(*particle, .7, 1);
				Vec2 drag = Force::generateDragForce(*particle, 0.4);

				if (particle->velocity.magnitude() >= 0.0) {
					particle->addForce(drag);
			
				}
				if (particle->velocity.y <= 0 && !particle->exploded) {
					particle->color = 0x00FFFFFF;
					particle->exploded = true;
					explosions.push_back(particle->position);
				}

			}
			if(t<76)
				checkBoundary(particle, Timestep);
		}

	}
	for (auto particle : eParticles) {
		particle->integrate(Timestep, Vec2(0, -30), 0);
		//Vec2 wind = Force::generateWindForce(*particle, .7, 1);
		Vec2 drag = Force::generateDragForce(*particle, 0.4);

		if (particle->velocity.magnitude() >= 0.0) {
			particle->addForce(drag);

		}
		if (particle->velocity.y <= -50)
		{
			int alpha = particle->color << 24;
			particle->color = 0x00000000;
		}
	}
	//collision for particle system
	for (auto v : explosions) {
		for (int i = 0; i < 30; i++) {
			Vec2 randomD = randomDirection();
			Particle* p = new Particle(v.x , v.y, 1, 0.3);
			p->velocity = randomD * randomFloat(1,6);
			p->color = 0xFF4B53A8;
			eParticles.push_back(p);
		}
	}
	explosions.clear();



	////collisions for single particle

	//Particle* collidedP = checkBoundaryCollisions();
	//if (collidedP!=nullptr) {
	//	Timestep = collidedP->fh * Timestep;
	//	//std::cout << "dt "<< Timestep << '\n';
	//	if (Timestep < 0.001) {
	//		collidedP->alive = false;
	//	}
	//	//revert state to before collision
	//	collidedP->acceleration = collidedP->oldAcc;
	//	collidedP->velocity = collidedP->oldVel;
	//	collidedP->position = collidedP->oldPos;
	//	//reintegrate particle at exact time of collision
	//	collidedP->integrate(Timestep);
	//	//std::cout << "hit plane at: " <<collidedP->position.x << ' ' << collidedP->position.y << '\n';
	//	//collision response
	//	boundaryCollisionResponse(collidedP);
	//	collidingP.erase(collidingP.begin());
	//}

	//TimestepRemaining = TimestepRemaining - Timestep;

//	}
	//std::cout << "frame done'\n";
	/*for (auto p : particles) {
		std::cout << p->position.x << " " << p->position.y << '\n';
	}*/




	////------------------ bouncing from the boundaries with reduced speed --------------------------
	//static float ratio = 0.0;
	//for (auto particle : particles) {
	//	if (Graphics::xPosInWindow(particle->position.x - particle->radius) <= 0) {
	//		particle->position.x = Graphics::xPosInCoordinate(0) + particle->radius;
	//		particle->velocity.x *= -ratio;
	//		particle->velocity.y *= ratio;
	//	}
	//	else if (Graphics::xPosInWindow(particle->position.x + particle->radius) >= Graphics::getWindowWidth()) {
	//		particle->position.x = Graphics::xPosInCoordinate(Graphics::getWindowWidth()) - particle->radius;
	//		particle->velocity.x *= -ratio;
	//		particle->velocity.y *= ratio;
	//	}
	//	if (Graphics::yPosInWindow(particle->position.y - particle->radius) >= Graphics::getWindowHeight()) {
	//		particle->position.y = Graphics::yPosInCoordinate(Graphics::getWindowHeight()) + particle->radius;
	//		particle->velocity.x *= ratio;
	//		particle->velocity.y *= -ratio;
	//	}
	//	else if (Graphics::yPosInWindow(particle->position.y + particle->radius) <= 0) {
	//		particle->position.y = Graphics::yPosInCoordinate(0) - particle->radius;
	//		particle->velocity.x *= ratio;
	//		particle->velocity.y *= -ratio;
	//	}
	//}
}
void Application::checkBoundary(Particle* p, float dt) {
	double cr = 0.3;
	double cf = 0.1;
	Vec2 floor{ -1,0 };
	Vec2 floorNorm{ 0,1 };
	Vec2 floorNunit = floorNorm.unitVector();
	float dn = (p->oldPos - floor).dot(floorNunit) + p->radius;
	//Vec2 newPos = p->position + p->velocity * dt;
	float dn1 = (p->position - floor).dot(floorNunit) - p->radius;
	//if dn sign and dn1 sign differ then collision
	if (!(dn * dn1 >= 0.0f)) {
		p->position = p->position - (floorNunit * ((1 + cr) * dn1));

		Vec2 velNorm = floorNunit * (p->velocity.dot(floorNunit));
		Vec2 velTan = p->velocity - velNorm;
		p->velocity = (velNorm * -cr) + (velTan * (1 - cf));
	}

}

////single ball collission
//void Application::boundaryCollisionResponse(Particle* p) {
//	Vec2 floor{ -1,0 };
//	Vec2 floorNorm{ 0,1 };
//	Vec2 floorNunit = floorNorm.unitVector();
//	Vec2 velNormPrev = floorNunit*(p->oldVel.dot(floorNunit));
//	Vec2 velTanPrev = p->oldVel - velNormPrev;
//	double cr = 0.3;
//	double cf = 0.1;
//	Vec2 velNormCur = velNormPrev * -cr;
//	Vec2 velTanCur = velTanPrev * (1 - cf);
//	p->velocity = velNormCur + velTanCur;
//}
////sinlge particle
//Particle* Application::checkBoundaryCollisions() {
//	Vec2 floor{ -1,0 };
//	Vec2 floorNorm{ 0,1 };
//	Vec2 floorNunit = floorNorm.unitVector();
//	//std::cout << floorNunit.x << " " << floorNunit.y << '\n';
//	for (auto particle : particles) {
//		if (particle->alive) {
//			//check sign
//			float dn = (particle->position - floor).dot(floorNunit) - particle->radius;
//			if (dn == 0 || dn < 0) {
//				//std::cout << "hit floor\n";
//				float dn1 = (particle->oldPos - floor).dot(floorNunit) + particle->radius;
//				particle->fh = (dn / (dn - dn1));
//				collidingP.push_back(particle);
//			}
//		}
//	}
//	std::sort(collidingP.begin(), collidingP.end(), [](Particle* one, Particle* two) {return one->fh < two->fh; });
//	if (collidingP.size() == 0)
//		return nullptr;
//	else
//		return collidingP.front();
//}
//====================================================================================//
// render - render the objects for a frame                                            //
//====================================================================================//
void Application::render(float dt) {
	Graphics::clearScreen(0xFF000000);
	if (dt <= 24) {
		//Graphics::coordinateGrid(Graphics::unitLength, 0xFF222222);	// draw the coordinate grid for reference
		for (auto particle : particles) {
			Graphics::drawDisk(particle->position.x, particle->position.y, particle->radius, particle->color);
		}
		//Graphics::drawDisk(particles[0]->position.x, particles[0]->position.y, particles[0]->radius, 0xFF555555);
		//Graphics::drawDisk(particles[1]->position.x, particles[1]->position.y, particles[1]->radius, 0xFF00FFFF);
		//SDL_Rect dstrect = { Graphics::xPosInWindow(-30), Graphics::yPosInWindow(70), 320, 240 };
		//SDL_RenderCopy(Graphics::renderer, texture, NULL, &dstrect);
	}
	else if (dt > 24 && dt <= 25) {

		for (auto particle : particles) {
			delete particle;
		}
		particles.clear();
	}
	else if (dt > 25 and dt<44) {
		Graphics::clearScreen(backgroundColor);
		//Graphics::coordinateGrid(Graphics::unitLength, 0xFF222222);	// draw the coordinate grid for reference
		for (auto particle : particles) {
			Graphics::drawDisk(particle->position.x, particle->position.y, particle->radius, particle->color);
		}
		for (auto body : bodies) {
			if (body->shape->getType() == DISK) {
				Disk* disk = (Disk*)body->shape;
				Graphics::drawDisk(body->position.x, body->position.y,
					disk->radius, body->rotation, body->color);
			}
			if (body->shape->getType() == BOX) {
				Box* box = (Box*)body->shape;
				if (body->color == 0) {
					SDL_Rect dstrect = { Graphics::xPosInWindow(body->position.x - box->width * 0.5),
					Graphics::yPosInWindow(body->position.y + box->height * 0.5),
					Graphics::unitLength * box->width, Graphics::unitLength * box->height };
					SDL_RenderCopyEx(Graphics::renderer, textures[0], NULL, &dstrect, body->rotation * -57.295779513, NULL, SDL_FLIP_NONE);
				}
				else
					Graphics::drawFilledPolygon(box->worldVertices, body->color);
			}
			/*	if (!body->isStatic() && body->shape->getType() == BOX) {
					Box* box = (Box*)body->shape;
					SDL_Rect dstrect = { Graphics::xPosInWindow(body->position.x - box->width * 0.5),
						Graphics::yPosInWindow(body->position.y + box->height * 0.5),
						Graphics::unitLength * box->width, Graphics::unitLength * box->height };
					SDL_RenderCopyEx(Graphics::renderer, texture, NULL, &dstrect, body->rotation * -57.295779513, NULL, SDL_FLIP_NONE);
				}*/
		}
		//Graphics::drawDisk(particles[0]->position.x, particles[0]->position.y, particles[0]->radius, 0xFF555555);
		//Graphics::drawDisk(particles[1]->position.x, particles[1]->position.y, particles[1]->radius, 0xFF00FFFF);
		//SDL_Rect dstrect = { Graphics::xPosInWindow(-30), Graphics::yPosInWindow(70), 320, 240 };
		//SDL_RenderCopy(Graphics::renderer, texture, NULL, &dstrect);

	}
	else if (dt >=44 && dt < 45) {
		Graphics::clearScreen(backgroundColor);

		for (auto body : bodies) {
			delete body;
		}
		bodies.clear();

		Body* rectangle = new Body(Box(15, 15), 0, 170, 500.0);
		rectangle->restitution = 0.9;
		rectangle->friction = 0.1;
		rectangle->color = squareColor;
		bodies.push_back(rectangle);

		Body* beam = new Body(Box(150, 10), -50, 150, -1);
		beam->restitution = 0.2;
		beam->friction = 0.1;
		beam->color = 0;
		beam->rotation = -3.14159 / 4;

		bodies.push_back(beam);

		Body* beam2 = new Body(Box(120, 10), 50, 50, -1);
		beam2->restitution = 0.5;
		beam2->friction = 0.1;
		beam2->color = 0;
		beam2->rotation = 3.14159 / 4;

		bodies.push_back(beam2);

		Body* beam3 = new Body(Box(150, 10), -80, 20, -1);
		beam3->restitution = 0.2;
		beam3->friction = 0.1;
		beam3->color = 0;
		beam3->rotation = -3.14159 / 4;

		bodies.push_back(beam3);
	}
	else if (dt >= 45 && dt<56) {
		Graphics::clearScreen(backgroundColor);
		for (auto body : bodies) {

			if (body->shape->getType() == BOX) {
				Box* box = (Box*)body->shape;
				if (body->color == 0) {
					SDL_Rect dstrect = { Graphics::xPosInWindow(body->position.x - box->width * 0.5),
					Graphics::yPosInWindow(body->position.y + box->height * 0.5),
					Graphics::unitLength * box->width, Graphics::unitLength * box->height };
					SDL_RenderCopyEx(Graphics::renderer, textures[0], NULL, &dstrect, body->rotation * -57.295779513, NULL, SDL_FLIP_NONE);
				}
				else
					Graphics::drawFilledPolygon(box->worldVertices, body->color);
			}
		}
	}
	else if (dt >= 56 && dt < 57) {
	Graphics::clearScreen(backgroundColor);
		for (auto body : bodies) {
			delete body;
		}
		bodies.clear();

		Body* rectangle = new Body(Box(15, 15), 25, 170, 500.0);
		rectangle->restitution = 0.9;
		rectangle->friction = 0.1;
		rectangle->rotation = -3.14159 / 4;
		rectangle->color = squareColor;
		bodies.push_back(rectangle);

		Body* rectangle2 = new Body(Box(15, 15), 0, 10, 500.0);
		rectangle2->restitution = 0.9;
		rectangle2->friction = 0.1;
		rectangle2->color = squareColor;
		bodies.push_back(rectangle2);

		Body* floor = new Body(Box(1000, 6),
			0, -3, -1);
		floor->restitution = 0.1;
		floor->friction = 0.5;
		floor->color = 0;
		bodies.push_back(floor);
	}
	else if (dt >= 57 && dt < 75) {
	Graphics::clearScreen(backgroundColor);
	for (auto particle : particles) {
		Graphics::drawDisk(particle->position.x, particle->position.y, particle->radius, particle->color);
	}
		for (auto body : bodies) {

			if (body->shape->getType() == BOX) {
				Box* box = (Box*)body->shape;
				if (body->color == 0) {
					SDL_Rect dstrect = { Graphics::xPosInWindow(body->position.x - box->width * 0.5),
					Graphics::yPosInWindow(body->position.y + box->height * 0.5),
					Graphics::unitLength * box->width, Graphics::unitLength * box->height };
					SDL_RenderCopyEx(Graphics::renderer, textures[0], NULL, &dstrect, body->rotation * -57.295779513, NULL, SDL_FLIP_NONE);
				}
				else
					Graphics::drawFilledPolygon(box->worldVertices, body->color);
			}
		}
	}
	else if (dt >= 75 && dt < 76) {
	
		for (auto particle : particles) {
			delete particle;
		}
		particles.clear();
	}
	else if (dt >= 76) {
	for (auto particle : eParticles) {
		Graphics::drawDisk(particle->position.x, particle->position.y, particle->radius, particle->color);
	}
		for (auto particle : particles) {
		
			if (particle->exploded) {
				//delete particle;
				//particles.erase(std::remove(particles.begin(), particles.end(), particle), particles.end());
			}
			else 
				Graphics::drawDisk(particle->position.x, particle->position.y, particle->radius, particle->color);
		}
	
	}
	Graphics::drawCircle(0, 50, 200, 0xFFFFFFFF);

	Graphics::renderFrame();

}

//====================================================================================//
// desdroy - release the resources                                                    //
//====================================================================================//
void Application::destroy() {
	//std::cout << particles.size()<<'\n';
	//std::ofstream outFile("my_file.txt");
	//for (const auto& e : particles ) outFile << e->position.x <<' '<< e->position.y << "\n";

	for (auto particle : particles) {
		delete particle;
	}
	for (auto particle : eParticles) {
		delete particle;
	}
	for (auto body : bodies) {
		delete body;
	}
	for (auto t : textures)
		SDL_DestroyTexture(t);
	Graphics::CloseWindow();
}

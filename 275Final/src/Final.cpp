#include "Application.h"
#include "./Physics/Constants.h"
#include "./Physics/Force.h"

#include <iostream>

SDL_Surface* image = NULL;
SDL_Texture* texture = NULL;

bool Application::isRunning() {
	return running;
}

//====================================================================================//
// setup - open a window, initialize objects, and set up coordinate system origin     //
//====================================================================================//
void Application::setup() {
	running = Graphics::openWindow("Austin Hartley Final",800,1000);//openWindow();

	//----------- set up the position of the coordinate system origin in the window -----------------
	Graphics::setOrigin(Graphics::windowWidth / 2, Graphics::windowHeight *.8);

	image = IMG_Load("./assets/basketball.png");
	SDL_Surface* formattedImage = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_ARGB8888, 0);
	if (image) {
		texture = SDL_CreateTextureFromSurface(Graphics::renderer, image);
		std::cout << "basket ball texture created" << std::endl;
		SDL_FreeSurface(image);
	}
	//std::cout << "bytes "<<(int)formattedImage->format->BytesPerPixel<<'\n';
	int w = formattedImage->w;
	int h = formattedImage->h;
	ParticleGenerator* gen = new ParticleGenerator(10);
	generators.push_back(gen);
	//std::cout << w << " " << h<<'\n';
	Uint32* pixels = (Uint32*)formattedImage->pixels;

	for (float y = 0; y < h; y++) {
		for (float x = 0; x < w; x++) {
			int index = (y * w + x);
			//std::cout << x << '\n';
			Uint8 r, g, b, a;
			SDL_GetRGBA(pixels[index], formattedImage->format, &r, &g, &b, &a);
			if (a > 0) {
				//Graphics::xPosInCoordinate(x + 200)
				//Particle* p = new Particle(x-20, y+50, 1, 1);
				//p->color = (a << 24) + (b << 16) + (g << 8) + r;
				//particles.push_back(p);
				int color = (a << 24) + (b << 16) + (g << 8) + r;
				gen->pos.push_back(Vec2{ x - 20,y + 50 });
				gen->colors.push_back(color);
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
		for (auto gen : generators) {
			gen->generateParticles(particles,t, Timestep);
		}

		//------------- Integrate the acceleration and velocity to estimate the new position ---------------
		for (auto particle : particles) {
			//implement: if particle needs to rest
			if (particle->alive) {
				particle->integrate(Timestep, Vec2(0, -30));
				Vec2 wind = Force::generateWindForce(*particle, 0.4, -12.5);
				Vec2 drag = Force::generateDragForce(*particle, 0.4);

				if (particle->velocity.magnitude() > 0.01) {
					particle->addForce(drag);
					particle->addForce(wind);
				}
				checkBoundary(particle,Timestep);
			}

		}
		//collision for particle system
		



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
	float dn = (p->oldPos - floor).dot(floorNunit)+p->radius ;
	//Vec2 newPos = p->position + p->velocity * dt;
	float dn1 = (p->position - floor).dot(floorNunit)-p->radius ;
	//if dn sign and dn1 sign differ then collision
	if (!(dn * dn1 >= 0.0f)) {
		p->position = p->position - (floorNunit * ((1 + cr) * dn1));

		Vec2 velNorm = floorNunit * (p->velocity.dot(floorNunit));
		Vec2 velTan = p->velocity - velNorm;
		p->velocity = (velNorm * -cr) + (velTan * (1 - cf));
	}

}

//single ball collission
void Application::boundaryCollisionResponse(Particle* p) {
	Vec2 floor{ -1,0 };
	Vec2 floorNorm{ 0,1 };
	Vec2 floorNunit = floorNorm.unitVector();
	Vec2 velNormPrev = floorNunit*(p->oldVel.dot(floorNunit));
	Vec2 velTanPrev = p->oldVel - velNormPrev;
	double cr = 0.3;
	double cf = 0.1;
	Vec2 velNormCur = velNormPrev * -cr;
	Vec2 velTanCur = velTanPrev * (1 - cf);
	p->velocity = velNormCur + velTanCur;
}
//sinlge particle
Particle* Application::checkBoundaryCollisions() {
	Vec2 floor{ -1,0 };
	Vec2 floorNorm{ 0,1 };
	Vec2 floorNunit = floorNorm.unitVector();
	//std::cout << floorNunit.x << " " << floorNunit.y << '\n';
	for (auto particle : particles) {
		if (particle->alive) {
			//check sign
			float dn = (particle->position - floor).dot(floorNunit) - particle->radius;
			if (dn == 0 || dn < 0) {
				//std::cout << "hit floor\n";
				float dn1 = (particle->oldPos - floor).dot(floorNunit) + particle->radius;
				particle->fh = (dn / (dn - dn1));
				collidingP.push_back(particle);
			}
		}
	}
	std::sort(collidingP.begin(), collidingP.end(), [](Particle* one, Particle* two) {return one->fh < two->fh; });
	if (collidingP.size() == 0)
		return nullptr;
	else
		return collidingP.front();
}
//====================================================================================//
// render - render the objects for a frame                                            //
//====================================================================================//
void Application::render() {
	Graphics::clearScreen(0xFF21070F);

	Graphics::coordinateGrid(Graphics::unitLength, 0xFF222222);	// draw the coordinate grid for reference
	for (auto particle : particles) {
		Graphics::drawDisk(particle->position.x, particle->position.y, particle->radius, particle->color);
	}
	//Graphics::drawDisk(particles[0]->position.x, particles[0]->position.y, particles[0]->radius, 0xFF555555);
	//Graphics::drawDisk(particles[1]->position.x, particles[1]->position.y, particles[1]->radius, 0xFF00FFFF);
	SDL_Rect dstrect = { Graphics::xPosInWindow(-30), Graphics::yPosInWindow(70), 320, 240};
	//SDL_RenderCopy(Graphics::renderer, texture, NULL, &dstrect);
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
	Graphics::CloseWindow();
}

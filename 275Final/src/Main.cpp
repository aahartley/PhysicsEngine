/*
The source code is from Gustavo Pezzi, co-founder of Pikuma.com
*/

//#include "App.h"
#include "Application.h"
#include <iostream>
//int main(int argc, char *args[]) {
//	Application app;
//
//	app.setup();
//
//	while (app.isRunning()) {
//		app.input();
//		app.update();
//		app.render();
//	}
//
//	app.destroy();
//
//	return 0;
//}

int main(int argc, char* args[]) {
	Application app;
	double EPSILON = 0.008;
	app.setup();
	double t = 0;
	int n = 0;
	double h = 0.046;

	SDL_AudioSpec wavSpec;
	Uint32 wavLength;
	Uint8* wavBuffer;

	SDL_LoadWAV("./assets/squaresF.wav", &wavSpec, &wavBuffer, &wavLength);
	// open audio device
	SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
	// play audio

	int success = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
	SDL_PauseAudioDevice(deviceId, 0);
	while (app.isRunning()  && t<90) {
		app.input();
		app.render(t);
		app.update(h,t);
		n = n + 1;
		t = n * h;
		//std::cout << t <<" "<< n << '\n';
		//std::cout << clock() << '\n';
	}
	// clean up
	std::cout << "done";
	SDL_PauseAudioDevice(deviceId, 1);

	SDL_CloseAudioDevice(deviceId);
	SDL_FreeWAV(wavBuffer);
	SDL_Quit();
	app.destroy();


	return 0;
}
/*
The source code is from Gustavo Pezzi, co-founder of Pikuma.com
*/

#include "Application.h"
#include <iostream>
int main(int argc, char *args[]) {
	Application app;

	app.setup();

	while (app.isRunning()) {
		app.input();
		app.update();
		app.render();
	}

	app.destroy();

	return 0;
}
//int main(int argc, char* args[]) {
//	Application app;
//	double EPSILON = 0.008;
//	app.setup();
//	double t = 0;
//	int n = 0;
//	double h = 0.046;
//	int output[6] = {0,1,2,3,4,5};
//	while (app.isRunning() /* && t<20*/) {
//		app.input();
//		app.render();
//		app.update(h);
//		n = n + 1;
//		t = n * h;
//		//std::cout << t <<" "<< n << '\n';
//		//std::cout << clock() << '\n';
//	}
//
//	app.destroy();
//
//	return 0;
//}
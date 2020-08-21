#pragma once
#include "Window.h"

class App {
public:
	App();

	int run();
private:
	void frameLoop();
private:
	Window wnd;
};
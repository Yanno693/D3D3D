#pragma once
#include "Window.h"
#include <chrono>

class App {
public:
	App();

	int run();
private:
	void frameLoop();
	int elapsedTime();

	std::chrono::time_point<std::chrono::system_clock> clock_start;

	float cameraPosition[3];
	float cameraRotation[3];

private:
	Window wnd;
};
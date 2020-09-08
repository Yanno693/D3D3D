#include <Windows.h>
#include "App.h"

App::App() : wnd(800, 600, L"D3D3D Project") {
	
	cameraPosition[0] = 0.0f;
	cameraPosition[1] = 0.0f;
	cameraPosition[2] = 0.0f;

	cameraRotation[0] = 0.0f;
	cameraRotation[1] = 0.0f;
	cameraRotation[2] = 0.0f;

	clock_start = std::chrono::system_clock::now();
}

int App::run() {
	while (true) {
		if (const auto ecode = Window::processMessages())
			return *ecode;
		
		frameLoop();
	}
}

int App::elapsedTime() {
	std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed = now - clock_start;

	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);

	return ms.count();
}

void App::frameLoop() {
	if (wnd.kb.KeyIsPressed(VK_UP))
		cameraPosition[1] -= 0.1f;
	
	if (wnd.kb.KeyIsPressed(VK_DOWN))
		cameraPosition[1] += 0.1f;

	if (wnd.kb.KeyIsPressed(VK_RIGHT))
		cameraPosition[0] -= 0.1f;

	if (wnd.kb.KeyIsPressed(VK_LEFT))
		cameraPosition[0] += 0.1f;


	//time_t now = time(0);

	float r = (float)(elapsedTime() % 500) / 500.f;
	float g = (float)(elapsedTime() % 800) / 800.f;

	//wnd.gfx().clearBuffer(r, g, 0);
	wnd.gfx().draw(
		(float)elapsedTime() / 1000.f,
		cameraPosition,
		cameraRotation);

	wnd.gfx().endFrame();
}
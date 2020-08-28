#include <Windows.h>
#include "App.h"

App::App() : wnd(640, 480, L"Window Name") {
	clock_start = std::chrono::system_clock::now();
}

int App::run() {
	//MSG msg;
	//BOOL gResult;

	//while ((gResult = GetMessage(&msg, nullptr, 0, 0) > 0)) {
	//	TranslateMessage(&msg);
	//	DispatchMessage(&msg);
	//	frameLoop();
	//}

	//if (gResult == -1) {
	//	throw D3D3D_LAST();
	//}

	//return msg.wParam;

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
	/*if (wnd.kb.KeyIsPressed('E')) {
		wnd.setTitle(L"lol");
	}*/

	//time_t now = time(0);

	float r = (float)(elapsedTime() % 500) / 500.f;
	float g = (float)(elapsedTime() % 800) / 800.f;

	wnd.gfx().clearBuffer(r, g, 0);
	wnd.gfx().drawTestTriangle((float)elapsedTime() / 1000.f);
	wnd.gfx().endFrame();
}
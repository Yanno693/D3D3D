#include "App.h"

App::App() : wnd(640, 480, L"Window Name") {}

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
		if (const auto ecode = Window::processMessages()) {
			return *ecode;
		}
		
		frameLoop();
	}
}

void App::frameLoop() {
	/*if (wnd.kb.KeyIsPressed('E')) {
		wnd.setTitle(L"lol");
	}*/
}
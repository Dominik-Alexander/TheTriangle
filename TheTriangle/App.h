#pragma once

#include <Windows.h>
#include "Graphics.h"

#include "Constants.h"

class App
{
private:
	HWND hwnd = { 0 };

	Graphics* pGraphics;

	int ticksLastFrame = 0;

	HWND CreateMainWindow(HINSTANCE hInstance);

	void Update(float deltaTime);
	void Render();
	void ReleaseWindow();

public:
	App();
	~App();

	HRESULT Initialize(HINSTANCE hInstance, int nCmdShow);
	void Run();
	void Shutdown();
};

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


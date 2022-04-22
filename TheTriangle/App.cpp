#include "App.h"

App::App()  
{
	pGraphics = new Graphics();
}

App::~App()
{

}

HRESULT App::Initialize(HINSTANCE hInstance, int nCmdShow)
{
	hwnd = CreateMainWindow(hInstance);

	if (!hwnd)
	{
		MessageBox(NULL, L"Could not create main window!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	pGraphics->Initialize(hwnd);

	ShowWindow(hwnd, nCmdShow);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);
	ShowCursor(false);

	return S_OK;
}

HWND App::CreateMainWindow(HINSTANCE hInstance)
{
	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WindowProc;
	wndClass.hInstance = hInstance;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = L"MainWindow";

	if (!RegisterClassEx(&wndClass))
		MessageBox(NULL, L"Could not register window class!", L"Error!", MB_ICONERROR);

	RECT windowDimension = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRect(&windowDimension, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX, FALSE);

	int windowPosX = (GetSystemMetrics(SM_CXSCREEN) - windowDimension.right - windowDimension.left) / 2;
	int windowPosY = (GetSystemMetrics(SM_CYSCREEN) - windowDimension.top - windowDimension.bottom) / 2;

	return CreateWindowA("MainWindow", "THE TRIANGLE",
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
		windowPosX,
		windowPosY,
		windowDimension.right - windowDimension.left,
		windowDimension.bottom - windowDimension.top,
		NULL,
		NULL,
		hInstance,
		NULL);
}

void App::Run()
{
	MSG msg = { 0 };

	int timeToWait = static_cast<int>((1000 / FPS) - (GetTickCount64() - ticksLastFrame));

	if (timeToWait > 0 && timeToWait <= (1000 / FPS))
	{
		Sleep(timeToWait);
	}

	float deltaTime = static_cast<float>((GetTickCount64() - ticksLastFrame) / 1000);

	deltaTime = (deltaTime > 0.05f) ? 0.05f : deltaTime;

	ticksLastFrame = static_cast<int>(GetTickCount64());

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		Update(deltaTime);

		Render();
	}
}

void App::Update(float deltaTime)
{

}

void App::Render()
{
	pGraphics->Render();
}

void App::Shutdown()
{
	pGraphics->Shutdown();

	if (pGraphics != NULL)
	{
		delete pGraphics;
		pGraphics = NULL;
	}

	ReleaseWindow();
}

void App::ReleaseWindow()
{
	if (hwnd)
	{
		DestroyWindow(hwnd);
		hwnd = NULL;
	}
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintStruct;
	HDC hdc;

	switch (uMsg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &paintStruct);
		EndPaint(hWnd, &paintStruct);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}
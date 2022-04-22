#include "App.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR cmdLine, _In_ int nCmdShow)
{
	App app;

	if (FAILED(app.Initialize(hInstance, nCmdShow)))
	{
		return -1;
	}

	app.Run();

	app.Shutdown();

	return 0;
}
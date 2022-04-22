#pragma once

#include "Direct3D11Class.h"

#include "Constants.h"

class Graphics
{
private:
	Direct3D11Class* pD3DObject = 0;

public:
	Graphics();
	~Graphics();

	HRESULT Initialize(HWND hwnd);
	void Shutdown();

	void Render();
};


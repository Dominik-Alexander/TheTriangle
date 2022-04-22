#include "Graphics.h"

Graphics::Graphics()
{
	
}

Graphics::~Graphics()
{

}

HRESULT Graphics::Initialize(HWND hwnd)
{
	pD3DObject = new Direct3D11Class();

	pD3DObject->Initialize(hwnd);

	return S_OK;
}

void Graphics::Shutdown()
{
	pD3DObject->Shutdown();

	if (pD3DObject != NULL)
	{
		delete pD3DObject;

		pD3DObject = NULL;
	}

}

void Graphics::Render()
{
	pD3DObject->Clear(CLEARCOLOR);

	pD3DObject->SetInputLayout();

	pD3DObject->SetVertexBuffers();

	pD3DObject->SetPrimitiveTopology();

	pD3DObject->SetVertexShader();
	pD3DObject->SetPixelShader();

	pD3DObject->DrawD3DContext();

	pD3DObject->RenderPresent();
}
#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "VertexPos.h"
#include "Constants.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

class Direct3D11Class
{
private:
	D3D_DRIVER_TYPE driverType;
	D3D_FEATURE_LEVEL featureLevel;

	ID3D11Device* d3dDevice;
	ID3D11DeviceContext* d3dContext;
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* backBufferTarget;

	ID3D11VertexShader* solidColorVS;
	ID3D11PixelShader* solidColorPS;

	ID3D11InputLayout* inputLayout;
	ID3D11Buffer* vertexBuffer;

	HRESULT LoadContent();
	void UnloadContent();

	HRESULT SetUpDeviceAndSwapchain(HWND hwnd);

	HRESULT SetUpRenderTargetView();

	HRESULT SetUpViewport();

public:
	Direct3D11Class();
	~Direct3D11Class();

	HRESULT Initialize(HWND hwnd);
	void Shutdown();

	void Clear(const float clearColor[4]);

	void SetInputLayout();

	void SetVertexBuffers();

	void SetPrimitiveTopology();

	void SetVertexShader();
	void SetPixelShader();

	void DrawD3DContext();

	void RenderPresent();

	HRESULT CompileShaderFromFile(LPCWSTR pathToShader, LPCSTR shaderEntryPoint, LPCSTR shaderModel, UINT compilerFlags, ID3DBlob** shaderBuffer, ID3DBlob** errorBuffer);
};


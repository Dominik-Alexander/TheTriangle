#include "Direct3D11Class.h"

Direct3D11Class::Direct3D11Class() : driverType(D3D_DRIVER_TYPE_NULL), featureLevel(D3D_FEATURE_LEVEL_11_0),
d3dDevice(0), d3dContext(0), swapChain(0), backBufferTarget(0), solidColorVS(0), solidColorPS(0),
inputLayout(0), vertexBuffer(0)
{

}

Direct3D11Class::~Direct3D11Class()
{

}

HRESULT Direct3D11Class::Initialize(HWND hwnd)
{
	SetUpDeviceAndSwapchain(hwnd);

	SetUpRenderTargetView();

	SetUpViewport();

	return S_OK;
}

HRESULT Direct3D11Class::SetUpDeviceAndSwapchain(HWND hwnd)
{
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE, D3D_DRIVER_TYPE_SOFTWARE
	};

	unsigned int totalDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};

	unsigned int totalFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = WINDOW_WIDTH;
	swapChainDesc.BufferDesc.Height = WINDOW_HEIGHT;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = true;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	unsigned int creationFlags = 0;

#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT result;
	unsigned int driver = 0;

	for (driver = 0; driver < totalDriverTypes; ++driver)
	{
		result = D3D11CreateDeviceAndSwapChain(0, driverTypes[driver], 0, creationFlags,
			featureLevels, totalFeatureLevels,
			D3D11_SDK_VERSION, &swapChainDesc, &swapChain,
			&d3dDevice, &featureLevel, &d3dContext);

		if (SUCCEEDED(result))
		{
			driverType = driverTypes[driver];
			break;
		}
	}

	if (FAILED(result))
	{
		MessageBox(NULL, L"Could not create device and swapchain!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	return S_OK;
}

HRESULT Direct3D11Class::SetUpRenderTargetView()
{
	ID3D11Texture2D* backBufferTexture;

	HRESULT result;

	result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTexture);

	if (FAILED(result))
	{
		MessageBox(NULL, L"Could not get backbuffer texture!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	result = d3dDevice->CreateRenderTargetView(backBufferTexture, 0, &backBufferTarget);

	if (backBufferTexture)
		backBufferTexture->Release();

	if (FAILED(result))
	{
		MessageBox(NULL, L"Could not create rendertargetview!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	d3dContext->OMSetRenderTargets(1, &backBufferTarget, 0);

	return S_FALSE;
}

HRESULT Direct3D11Class::SetUpViewport()
{
	D3D11_VIEWPORT viewport;
	viewport.Width = static_cast<float>(WINDOW_WIDTH);
	viewport.Height = static_cast<float>(WINDOW_HEIGHT);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	d3dContext->RSSetViewports(1, &viewport);

	HRESULT result = LoadContent();

	if (FAILED(result))
	{
		return S_FALSE;
	}

	return S_OK;

	return S_FALSE;
}

HRESULT Direct3D11Class::LoadContent()
{
	ID3DBlob* vsBuffer = 0;
	ID3DBlob* vsErrorBuffer = 0;

	if (FAILED(CompileShaderFromFile(L"Shaders/VertexShader.hlsl", "VS_Main", "vs_5_0", D3DCOMPILE_DEBUG, &vsBuffer, &vsErrorBuffer)))
	{
		MessageBox(NULL, L"Could not compile vertex shader!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	HRESULT d3dResult;

	d3dResult = d3dDevice->CreateVertexShader(vsBuffer->GetBufferPointer(),
		vsBuffer->GetBufferSize(), 0, &solidColorVS);

	if (FAILED(d3dResult))
	{
		if (vsBuffer)
			vsBuffer->Release();

		return d3dResult;
	}

	D3D11_INPUT_ELEMENT_DESC solidColorLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	unsigned int totalLayoutElements = ARRAYSIZE(solidColorLayout);

	d3dResult = d3dDevice->CreateInputLayout(solidColorLayout, totalLayoutElements,
		vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), &inputLayout);

	vsBuffer->Release();

	if (FAILED(d3dResult))
	{
		return S_FALSE;
	}

	ID3DBlob* psBuffer = 0;
	ID3DBlob* psErrorBuffer = 0;

	if (FAILED(CompileShaderFromFile(L"Shaders/PixelShader.hlsl", "PS_Main", "ps_5_0", D3DCOMPILE_DEBUG, &psBuffer, &psErrorBuffer)))
	{
		MessageBox(NULL, L"Could not compile pixel shader!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	d3dResult = d3dDevice->CreatePixelShader(psBuffer->GetBufferPointer(),
		psBuffer->GetBufferSize(), 0, &solidColorPS);

	psBuffer->Release();

	if (FAILED(d3dResult))
	{
		return S_FALSE;
	}

	VertexPos vertices[] =
	{
		DirectX::XMFLOAT3(0.0f,  0.75f, 0.75f),
		DirectX::XMFLOAT3(0.75f, -0.75f, 0.75f),
		DirectX::XMFLOAT3(-0.75f, -0.75f, 0.75f)
	};

	D3D11_BUFFER_DESC vertexDesc;
	ZeroMemory(&vertexDesc, sizeof(vertexDesc));
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.ByteWidth = sizeof(VertexPos) * 3;

	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(resourceData));
	resourceData.pSysMem = vertices;

	d3dResult = d3dDevice->CreateBuffer(&vertexDesc, &resourceData, &vertexBuffer);

	if (FAILED(d3dResult))
	{
		return S_FALSE;
	}

	return S_OK;
}

void Direct3D11Class::Shutdown()
{
	UnloadContent();

	if (backBufferTarget) backBufferTarget->Release();
	if (swapChain) swapChain->Release();
	if (d3dContext) d3dContext->Release();
	if (d3dDevice) d3dDevice->Release();

	backBufferTarget = 0;
	swapChain = 0;
	d3dContext = 0;
	d3dDevice = 0;
}

void Direct3D11Class::UnloadContent()
{
	if (solidColorVS) solidColorVS->Release();
	if (solidColorPS) solidColorPS->Release();
	if (inputLayout) inputLayout->Release();
	if (vertexBuffer) vertexBuffer->Release();

	solidColorVS = 0;
	solidColorPS = 0;
	inputLayout = 0;
	vertexBuffer = 0;
}

void Direct3D11Class::Clear(const float clearColor[4])
{
	d3dContext->ClearRenderTargetView(backBufferTarget, clearColor);
}

void Direct3D11Class::SetInputLayout()
{
	d3dContext->IASetInputLayout(inputLayout);
}

void Direct3D11Class::SetVertexBuffers()
{
	unsigned int stride = sizeof(VertexPos);
	unsigned int offset = 0;

	d3dContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
}

void Direct3D11Class::SetPrimitiveTopology()
{
	d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Direct3D11Class::SetVertexShader()
{
	d3dContext->VSSetShader(solidColorVS, 0, 0);
}

void Direct3D11Class::SetPixelShader()
{
	d3dContext->PSSetShader(solidColorPS, 0, 0);
}

void Direct3D11Class::DrawD3DContext()
{
	d3dContext->Draw(3, 0);
}

void Direct3D11Class::RenderPresent()
{
	swapChain->Present(0, 0);
}

HRESULT Direct3D11Class::CompileShaderFromFile(LPCWSTR pathToShader, LPCSTR shaderEntryPoint, LPCSTR shaderModel, UINT compilerFlags, ID3DBlob** shaderBuffer, ID3DBlob** errorBuffer)
{
	HRESULT compileResult = D3DCompileFromFile(pathToShader, NULL, NULL, shaderEntryPoint, shaderModel, compilerFlags, NULL, shaderBuffer, errorBuffer);

	if (FAILED(compileResult))
	{
		MessageBox(NULL, L"Could not compile shader!", L"Error!", MB_ICONERROR);
		return S_FALSE;
	}

	return S_OK;
}
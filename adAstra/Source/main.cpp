#include <Windows.h>
#include <dinput.h>

#include <main.h>
#include <Mesh.h>
#include <Misc.h>
#include <Background.h>
#include <Universe.h>
#include <ShaderCollection.h>

#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "GraphicsLibriary.lib")

#pragma region Globals

HINSTANCE               hInst = NULL;
HWND                    hWnd = NULL;
IDirectInput8*          directInput;
IDirectInputDevice8*    keyboard;
ID3D11Device*           device = NULL;
ID3D11DeviceContext*    deviceContext = NULL;
IDXGISwapChain*         swapChain = NULL;
ID3D11RenderTargetView* targetView = NULL;
ID3D11Texture2D*        depthStencil = NULL;
ID3D11DepthStencilView* depthStencilView = NULL;
ID3D11Buffer*           viewTransformBuffer = NULL;

Universe universe;
Player* player;

#pragma endregion

#pragma region Forward Declarations

LRESULT WINAPI manageMessages(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
HRESULT initializeWindow(HINSTANCE hInstance, int nCmdShow);
HRESULT initializeDirectX11();
HRESULT createDeviceAndSwapChain();
HRESULT setPipelineOutput();
void handleKeyboardInput();
void initializeWorld();
bool checkMessages();
void gameLoop();
void renderScene();
void cleanup();

#pragma endregion

struct ViewTransform
{
	matrix view;
	matrix projection;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (FAILED(initializeWindow(hInstance, nCmdShow)))
	{
		MessageBox(hWnd, L"Failed to initialize window.", L"Critical Error", 0);
		return 0;
	}

	if (FAILED(initializeDirectX11()))
	{
		MessageBox(hWnd, L"Failed to initialize DirectX.", L"Critical Error", 0);
		return 0;
	}
	gameLoop();
	return 0;
}

HRESULT initializeWindow(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;

	WNDCLASS  wc;
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = manageMessages;
	wc.cbClsExtra = wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) 6;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"GameWindow";
	RegisterClass(&wc);

	hWnd = CreateWindow(L"GameWindow", L"adAstra", WS_OVERLAPPEDWINDOW, 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

	if (!hWnd) return E_FAIL;
	ShowWindow(hWnd, nCmdShow);
	return S_OK;
}

HRESULT initializeDirectX11()
{
	createDeviceAndSwapChain();
	setPipelineOutput();

	srand(GetTickCount());

	D3D11_RASTERIZER_DESC wfdesc;
	ID3D11RasterizerState* WireFrame;
	ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
	wfdesc.FillMode = D3D11_FILL_SOLID;
	wfdesc.CullMode = D3D11_CULL_NONE;
	device->CreateRasterizerState(&wfdesc, &WireFrame);
	deviceContext->RSSetState(WireFrame);

	DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**) &directInput, NULL);
	directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);

	keyboard->SetDataFormat(&c_dfDIKeyboard);
	keyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	keyboard->Acquire();

	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(ViewTransform);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	device->CreateBuffer(&bd, NULL, &viewTransformBuffer);

	ViewTransform buf;
	XMVECTOR pos = XMVectorSet(0, 0, -2, 0);
	XMVECTOR lookAt = XMVectorSet(0, 0, 0, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	buf.projection = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XM_PIDIV2, 16.0 / 9, 0.01f, 100000.0f));
	buf.view = XMMatrixTranspose(XMMatrixLookAtLH(pos, lookAt, up));
	deviceContext->VSSetConstantBuffers(0, 1, &viewTransformBuffer);
	deviceContext->UpdateSubresource(viewTransformBuffer, 0, NULL, &buf, 0, 0);

	return S_OK;
}

void initializeWorld()
{
	player = new Player();
	universe.Load(device);
	universe.AddPlayer(player);
}

HRESULT createDeviceAndSwapChain()
{
	RECT rc;
	GetClientRect(hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	return D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, 0, D3D11_SDK_VERSION,
		&sd, &swapChain, &device, NULL, &deviceContext);
}

HRESULT setPipelineOutput()
{
	ID3D11Texture2D* backBuffer = NULL;
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*) &backBuffer);

	device->CreateRenderTargetView(backBuffer, NULL, &targetView);
	backBuffer->Release();

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = WINDOW_WIDTH;
	descDepth.Height = WINDOW_HEIGHT;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	device->CreateTexture2D(&descDepth, NULL, &depthStencil);

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	device->CreateDepthStencilView(depthStencil, &descDSV, &depthStencilView);

	deviceContext->OMSetRenderTargets(1, &targetView, depthStencilView);

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT) WINDOW_WIDTH;
	vp.Height = (FLOAT) WINDOW_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	deviceContext->RSSetViewports(1, &vp);

	return S_OK;
}

void gameLoop()
{
	initializeWorld();
	while (checkMessages())
	{
		keyboard->Acquire();
		handleKeyboardInput();

		universe.Update();

		renderScene();
	}
	cleanup();
}

void handleKeyboardInput()
{
	char buffer[256];
	keyboard->GetDeviceState(sizeof(buffer), buffer);

	if (buffer[DIK_ESCAPE])
		PostQuitMessage(0);

	if (buffer[DIK_SPACE])
		player->Push();

	if (buffer[DIK_A])
		player->RotateCW();

	if (buffer[DIK_D])
		player->RotateCCW();
}

void renderScene()
{
	float ClearColor[4] = { 0.3f, 0.3f, 1.0f, 1.0f };
	deviceContext->ClearRenderTargetView(targetView, ClearColor);
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	universe.Render(deviceContext);

	swapChain->Present(0, 0);
}

void cleanup()
{
	if (deviceContext) deviceContext->ClearState();

	universe.Release();

	if (keyboard) keyboard->Release();
	if (directInput) directInput->Release();
	if (depthStencil) depthStencil->Release();
	if (depthStencilView) depthStencilView->Release();
	if (targetView) targetView->Release();
	if (swapChain) swapChain->Release();
	if (deviceContext) deviceContext->Release();
	if (device) device->Release();
}

bool checkMessages()
{
	MSG msg;
	if (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
			return false;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return true;
}

LRESULT WINAPI manageMessages(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}
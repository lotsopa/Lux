#include "LuxPCH.h"
#include "LuxKeyCodes.h"
#include "LuxRenderWindow.h"
#include "LuxEventListener.h"
#include "LuxEventListenerDX11.h"
#include "LuxRenderWindowDX11.h"
#include "LuxKey.h"
#include "LuxShader.h"
#include "LuxSubMesh.h"

// TODO

Lux::Core::Internal::RenderWindowDX11::~RenderWindowDX11()
{
	Utility::SafePtrDelete(m_Input);

	// close and release all existing COM objects
	m_SwapChain.Reset();
	m_Device.Reset();
	m_DeviceContext.Reset();
	m_RenderBackbuffer.Reset();
	m_DepthStencilBuffer.Reset();
	m_DepthStencilState.Reset();
	m_DepthStencilView.Reset();
}

bool Lux::Core::Internal::RenderWindowDX11::Initialize(Utility::AppInitOptions& a_AppInitOptions)
{
	m_Width = a_AppInitOptions.m_WindowWidth;
	m_Height = a_AppInitOptions.m_WindowHeight;
	m_Fullscreen = a_AppInitOptions.m_FullScreen;
	m_SwapInterval = a_AppInitOptions.m_ScreenSwapInterval;
	m_Caption = a_AppInitOptions.m_WindowCaption;
	m_AntiAliasing = a_AppInitOptions.m_AntiAliasing;

	// Get the instance of this application.
	m_AppInstance = GetModuleHandle(NULL);

	// fill in the struct with the needed information
	m_WindowClass.cbSize = sizeof(WNDCLASSEX);
	m_WindowClass.style = CS_HREDRAW | CS_VREDRAW;
	m_WindowClass.lpfnWndProc = Lux::Core::Internal::RenderWindowDX11::WndProc;
	m_WindowClass.hInstance = m_AppInstance;
	m_WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	m_WindowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	m_WindowClass.lpszClassName = a_AppInitOptions.m_WindowCaption.c_str();

	// register the window class
	RegisterClassEx(&m_WindowClass);

	if (m_Fullscreen)
	{
		DEVMODE dmScreenSettings;
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)m_Width;
		dmScreenSettings.dmPelsHeight = (unsigned long)m_Height;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
	}

	// Create window
	m_WindowHandle = CreateWindowEx(WS_EX_APPWINDOW,a_AppInitOptions.m_WindowCaption.c_str(), a_AppInitOptions.m_WindowCaption.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, m_Width, m_Height, nullptr, nullptr, nullptr, nullptr);

	if (!m_WindowHandle)
	{
		return false;
	}

	// Set the user pointer, so we have access to the Window class inside the message loop
	SetWindowLong(m_WindowHandle, GWLP_USERDATA, (long)this);
	ShowWindow(m_WindowHandle, SW_SHOW);
	SetForegroundWindow(m_WindowHandle);
	SetFocus(m_WindowHandle);

	// Show the mouse cursor.
	ShowCursor(true);

	// Create a default input event listener
	m_Input = new EventListenerDX11(this);

	// Initialize DirectX
	bool dxInit = InitDX11();

	if (!dxInit)
		return false;

	return true;
}

Lux::Core::EventListener* Lux::Core::Internal::RenderWindowDX11::GetEventListener()
{
	return m_Input;
}

void Lux::Core::Internal::RenderWindowDX11::SwapBuffers()
{
	HRESULT hr;
	hr = m_SwapChain->Present(m_SwapInterval, 0);

	if (FAILED(hr))
	{
		Utility::ThrowError("Error Swapping Back Buffer");
	}
}

void Lux::Core::Internal::RenderWindowDX11::Clear()
{
	RECT rect;
	if (GetWindowRect(m_WindowHandle, &rect))
	{
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;

		if (width != m_Width || height != m_Height)
		{
			m_Width = width;
			m_Height = height;
			ResizeSwapChain(m_Width, m_Height);
			m_WindowResized = true;
		}
		else
		{
			m_WindowResized = false;
		}
	}
	m_DeviceContext->ClearRenderTargetView(m_RenderBackbuffer.Get(), value_ptr(WINDOW_CLEAR_COLOR));
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, NULL);

	ResetViewPortTargets();
}

void Lux::Core::Internal::RenderWindowDX11::PollEvents()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Lux::Core::Internal::RenderWindowDX11::Render(SubMesh* a_SubMesh)
{
	m_DeviceContext->DrawIndexed(a_SubMesh->GetNumIndices(), 0, 0);
}

const bool Lux::Core::Internal::RenderWindowDX11::IsWindowResized()
{
	return m_WindowResized;
}

const int Lux::Core::Internal::RenderWindowDX11::GetHeight()
{
	return m_Height;
}

const int Lux::Core::Internal::RenderWindowDX11::GetWidth()
{
	return m_Width;
}

Lux::Core::Internal::RenderWindowDX11::RenderWindowDX11() :
m_WindowHandle(nullptr), m_Input(nullptr), m_WindowResized(false)
{
	ZeroMemory(&m_WindowClass, sizeof(WNDCLASSEX));
}

LRESULT CALLBACK Lux::Core::Internal::RenderWindowDX11::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RenderWindowDX11* thisWindow = (RenderWindowDX11*)GetWindowLong(hwnd, GWLP_USERDATA);
	switch (message)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		thisWindow->m_ShouldQuit = true;
		return 0;
	}

		// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		thisWindow->m_ShouldQuit = true;
		return 0;
	}

	default:
	{
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	}
}

bool Lux::Core::Internal::RenderWindowDX11::InitDX11()
{
	HRESULT hr = 0;

	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = m_WindowHandle;                                // the window to be used
	scd.SampleDesc.Count = m_AntiAliasing;                               // how many multisamples
	scd.Windowed = !m_Fullscreen;                                    // windowed/full-screen mode

	// create a device, device context and swap chain using the information in the scd struct
	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION,
		&scd, &m_SwapChain, &m_Device, NULL, &m_DeviceContext);

	if (FAILED(hr))
	{
		return false;
	}

	// get the address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	if (FAILED(hr))
	{
		return false;
	}

	// use the back buffer address to create the render target
	hr = m_Device->CreateRenderTargetView(pBackBuffer, NULL, m_RenderBackbuffer.GetAddressOf());
	pBackBuffer->Release();

	if (FAILED(hr))
	{
		return false;
	}

	// set the render target as the back buffer
	m_DeviceContext->OMSetRenderTargets(1, m_RenderBackbuffer.GetAddressOf(), NULL);

	// Set the viewport
	ZeroMemory(&m_ViewPort, sizeof(D3D11_VIEWPORT));

	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;
	m_ViewPort.Width = (float)m_Width;
	m_ViewPort.Height = (float)m_Height;
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;

	// Setup depth/stencil state.
	D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
	ZeroMemory(&depthStencilStateDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depthStencilStateDesc.DepthEnable = TRUE;
	depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilStateDesc.StencilEnable = FALSE;

	hr = m_Device->CreateDepthStencilState(&depthStencilStateDesc, m_DepthStencilState.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	// Create the depth buffer for use with the depth/stencil view.
	D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
	ZeroMemory(&depthStencilBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));

	depthStencilBufferDesc.ArraySize = 1;
	depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilBufferDesc.CPUAccessFlags = 0; // No CPU access required.
	depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilBufferDesc.Width = m_Width;
	depthStencilBufferDesc.Height = m_Height;
	depthStencilBufferDesc.MipLevels = 1;
	depthStencilBufferDesc.SampleDesc.Count = m_AntiAliasing;
	depthStencilBufferDesc.SampleDesc.Quality = 0;
	depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	hr = m_Device->CreateTexture2D(&depthStencilBufferDesc, nullptr, m_DepthStencilBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	hr = m_Device->CreateDepthStencilView(m_DepthStencilBuffer.Get(), nullptr, m_DepthStencilView.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	// Setup rasterizer state.
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

	rasterizerDesc.AntialiasedLineEnable = TRUE;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = TRUE;
	rasterizerDesc.MultisampleEnable = TRUE;
	rasterizerDesc.ScissorEnable = FALSE;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state object.
	hr = m_Device->CreateRasterizerState(&rasterizerDesc, &m_Rasterizer);

	if (FAILED(hr))
	{
		return false;
	}

	ResetViewPortTargets();
	
	return true;
}

void Lux::Core::Internal::RenderWindowDX11::ResetViewPortTargets()
{
	m_DeviceContext->RSSetState(m_Rasterizer.Get());
	m_DeviceContext->RSSetViewports(1, &m_ViewPort);
	m_DeviceContext->OMSetRenderTargets(1, m_RenderBackbuffer.GetAddressOf(), m_DepthStencilView.Get());
	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState.Get(), 0);
}

void Lux::Core::Internal::RenderWindowDX11::ResizeSwapChain(int width, int height)
{
	// Don't allow for 0 size swap chain buffers.
	if (width <= 0) width = 1;
	if (height <= 0) height = 1;

	m_DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);

	// First release the render target and depth/stencil views.
	m_RenderBackbuffer.Reset();
	m_DepthStencilBuffer.Reset();
	m_DepthStencilView.Reset();

	// Resize the swap chain buffers.
	m_SwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	// Next initialize the back buffer of the swap chain and associate it to a 
	// render target view.
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	HRESULT hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer);
	if (FAILED(hr))
	{
		Utility::ThrowError("Could not initialize new back buffer.");
	}

	hr = m_Device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_RenderBackbuffer.GetAddressOf());
	if (FAILED(hr))
	{
		Utility::ThrowError("Could not create render target view.");
	}

	backBuffer.Reset();

	// Create the depth buffer for use with the depth/stencil view.
	D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
	ZeroMemory(&depthStencilBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));

	depthStencilBufferDesc.ArraySize = 1;
	depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilBufferDesc.CPUAccessFlags = 0; // No CPU access required.
	depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilBufferDesc.Width = width;
	depthStencilBufferDesc.Height = height;
	depthStencilBufferDesc.MipLevels = 1;
	depthStencilBufferDesc.SampleDesc.Count = m_AntiAliasing;
	depthStencilBufferDesc.SampleDesc.Quality = 0;
	depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	hr = m_Device->CreateTexture2D(&depthStencilBufferDesc, nullptr, m_DepthStencilBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		Utility::ThrowError("Could not create depth stencil buffer.");
	}

	hr = m_Device->CreateDepthStencilView(m_DepthStencilBuffer.Get(), nullptr, m_DepthStencilView.GetAddressOf());
	if (FAILED(hr))
	{
		Utility::ThrowError("Could not create depth stencil view.");
	}

	ZeroMemory(&m_ViewPort, sizeof(D3D11_VIEWPORT));

	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;
	m_ViewPort.Width = (float)width;
	m_ViewPort.Height = (float)height;
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;
}

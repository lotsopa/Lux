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
	m_Backbuffer->Release();
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

	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = m_WindowHandle;                                // the window to be used
	scd.SampleDesc.Count = a_AppInitOptions.m_AntiAliasing;                               // how many multisamples
	scd.Windowed = !m_Fullscreen;                                    // windowed/full-screen mode

	// create a device, device context and swap chain using the information in the scd struct
	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION,
		&scd, &m_SwapChain, &m_Device, NULL, &m_DeviceContext);

	// get the address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// use the back buffer address to create the render target
	m_Device->CreateRenderTargetView(pBackBuffer, NULL, &m_Backbuffer);
	pBackBuffer->Release();

	// set the render target as the back buffer
	m_DeviceContext->OMSetRenderTargets(1, &m_Backbuffer, NULL);

	// Set the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)m_Width;
	viewport.Height = (float)m_Height;

	m_DeviceContext->RSSetViewports(1, &viewport);

	return true;
}

Lux::Core::EventListener* Lux::Core::Internal::RenderWindowDX11::GetEventListener()
{
	return m_Input;
}

void Lux::Core::Internal::RenderWindowDX11::SwapBuffers()
{
	m_SwapChain->Present(m_SwapInterval, 0);
}

void Lux::Core::Internal::RenderWindowDX11::Clear()
{
	m_DeviceContext->ClearRenderTargetView(m_Backbuffer, value_ptr(WINDOW_CLEAR_COLOR));
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

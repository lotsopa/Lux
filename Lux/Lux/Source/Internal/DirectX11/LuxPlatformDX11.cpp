#include "LuxPCH.h"
#include "LuxHelpers.h"
#include "LuxRenderWindow.h"
#include "LuxRenderWindowDX11.h"
#include "LuxPlatform.h"
#include "LuxPlatformDX11.h"

// TODO

Lux::Core::Internal::PlatformDX11::~PlatformDX11()
{
	Utility::SafePtrDelete(m_RenderWindow);
}

bool Lux::Core::Internal::PlatformDX11::Init(Utility::AppInitOptions& a_AppInitOptions)
{
	m_RenderWindow = new RenderWindowDX11();

	bool windowInit = m_RenderWindow->Initialize(a_AppInitOptions);

	if (!windowInit)
	{
		return false;
	}
	return true;
}

Lux::Core::RenderWindow* Lux::Core::Internal::PlatformDX11::GetRenderWindow()
{
	return m_RenderWindow;
}

const bool Lux::Core::Internal::PlatformDX11::ShouldQuit()
{
	return m_RenderWindow->ShouldQuit();
}

Lux::Core::Internal::PlatformDX11::PlatformDX11() : m_RenderWindow(nullptr)
{

}

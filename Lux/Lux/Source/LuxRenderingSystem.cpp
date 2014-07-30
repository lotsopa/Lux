#include "LuxPCH.h"
#include "LuxRenderWindow.h"
#include "LuxEntity.h"
#include "LuxTransform.h"
#include "LuxKey.h"
#include "LuxMesh.h"
#include "LuxObjectPool.h"
#include "LuxComponentFactory.h"
#include "LuxEntityFactory.h"
#include "LuxSceneManager.h"
#include "LuxSystem.h"
#include "LuxRenderingSystem.h"

Lux::RenderingSystem::RenderingSystem()
{

}

Lux::RenderingSystem::~RenderingSystem()
{

}

void Lux::RenderingSystem::ProcessUpdate(const float a_DeltaTime)
{
	glfwSwapBuffers(m_RenderWindow->GetWindowHandle());
}

bool Lux::RenderingSystem::Init(SceneManager* a_SceneManager)
{
	System::Init(a_SceneManager);
	m_RenderWindow = m_SceneManager->GetRenderWindow();
	return true;
}

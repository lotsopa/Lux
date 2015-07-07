#include "LuxPCH.h"
#include "LuxObjectPool.h"
#include "LuxComponent.h"
#include "LuxEntity.h"
#include "LuxTransform.h"
#include "LuxKey.h"
#include "LuxModel.h"
#include "LuxObjectPool.h"
#include "LuxComponentFactory.h"
#include "LuxEntityFactory.h"
#include "LuxSceneManager.h"
#include "LuxSystem.h"

Lux::Core::System::System() : m_SceneManager(nullptr)
{

}

Lux::Core::System::~System()
{

}

bool Lux::Core::System::Init(SceneManager* a_SceneManager)
{
	LuxAssert(a_SceneManager);

	if (a_SceneManager == nullptr)
	{
		return false;
	}

	m_SceneManager = a_SceneManager;
	return true;
}

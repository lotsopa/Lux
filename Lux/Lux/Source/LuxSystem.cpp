#include "LuxPCH.h"
#include "LuxEntity.h"
#include "LuxTransform.h"
#include "LuxKey.h"
#include "LuxMesh.h"
#include "LuxObjectPool.h"
#include "LuxComponentFactory.h"
#include "LuxEntityFactory.h"
#include "LuxSceneManager.h"
#include "LuxSystem.h"

Lux::System::System()
{

}

Lux::System::~System()
{

}

bool Lux::System::Init(SceneManager* a_SceneManager)
{
	LuxAssert(a_SceneManager);

	if (a_SceneManager == nullptr)
	{
		return false;
	}

	m_SceneManager = a_SceneManager;
	return true;
}

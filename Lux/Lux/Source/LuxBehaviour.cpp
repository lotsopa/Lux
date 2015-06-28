#include "LuxPCH.h"
#include "LuxKeyCodes.h"
#include "LuxObjectPool.h"
#include "LuxEntity.h"
#include "LuxComponent.h"
#include "LuxTransform.h"
#include "LuxMaterialResource.h"
#include "LuxMaterial.h"
#include "LuxKey.h"
#include "LuxMesh.h"
#include "LuxLight.h"
#include "LuxObjectPool.h"
#include "LuxComponentFactory.h"
#include "LuxEntityFactory.h"
#include "LuxSystem.h"
#include "LuxShader.h"
#include "LuxRenderingSystem.h"
#include "LuxBehaviourSystem.h"
#include "LuxEventSystem.h"
#include "LuxSystemFactory.h"
#include "LuxSceneManager.h"
#include "LuxBehaviour.h"
#include "LuxFreeLookCamera.h"

Lux::Core::Behaviour::Behaviour() : Component(), m_Enabled(true), m_EventListener(nullptr), m_ComponentLayout(nullptr)
{

}

Lux::Core::Behaviour::~Behaviour()
{

}

void Lux::Core::Behaviour::Init(InitOptions& a_InitOpts)
{
	m_EventListener = a_InitOpts.m_EventListener;
}

const unsigned int Lux::Core::Behaviour::GetComponentIndex(const Key& a_Key)
{
	return m_ComponentLayout->m_SceneManager->m_ComponentIndexMap.at(a_Key);
}

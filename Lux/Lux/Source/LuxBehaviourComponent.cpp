#include "LuxPCH.h"
#include "LuxObjectPool.h"
#include "LuxKeyCodes.h"
#include "LuxComponent.h"
#include "LuxEventListener.h"
#include "LuxKey.h"
#include "LuxBehaviourComponent.h"

Lux::Core::BehaviourComponent::BehaviourComponent() : Component(), m_Enabled(true), m_EventListener(nullptr)
{

}

Lux::Core::BehaviourComponent::~BehaviourComponent()
{

}

void Lux::Core::BehaviourComponent::Init(InitOptions& a_InitOpts)
{
	m_EventListener = a_InitOpts.m_EventListener;
	m_Transform = a_InitOpts.m_Transform;
}

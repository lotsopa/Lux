#include "LuxPCH.h"
#include "LuxComponent.h"
#include "LuxEntity.h"
#include "LuxObjectPool.h"
#include "LuxEntityFactory.h"

Lux::Core::EntityFactory::EntityFactory(const unsigned int a_InitialPoolSize, const unsigned int a_GrowSize) :
m_EntityMemoryPool(a_InitialPoolSize, a_GrowSize)
{

}

Lux::Core::EntityFactory::~EntityFactory()
{

}

Lux::Core::Entity* Lux::Core::EntityFactory::CreateEntity()
{
	Entity* ent = m_EntityMemoryPool.CreateObject();

	if (ent == nullptr)
	{
		Utility::ThrowError("Fatal error. Could not create Entity.");
	}
	ent->Reset();
	return ent;
}

bool Lux::Core::EntityFactory::DestroyEntity(Entity* a_Entity)
{
	return m_EntityMemoryPool.DeleteObject(a_Entity);
}

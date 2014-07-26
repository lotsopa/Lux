#include "LuxPCH.h"
#include "LuxComponent.h"
#include "LuxEntity.h"
#include "LuxObjectPool.h"
#include "LuxEntityFactory.h"

Lux::EntityFactory::EntityFactory(const unsigned int a_InitialPoolSize, const unsigned int a_GrowSize) :
m_EntityMemoryPool(a_InitialPoolSize, a_GrowSize)
{

}

Lux::EntityFactory::~EntityFactory()
{

}

Lux::Entity* Lux::EntityFactory::CreateEntity()
{
	Entity* ent = m_EntityMemoryPool.CreateObject();

	if (ent == nullptr)
	{
		ThrowError("Fatal error. Could not create Entity.");
	}
	ent->Reset();
	return ent;
}

bool Lux::EntityFactory::DestroyEntity(Entity* a_Entity)
{
	return m_EntityMemoryPool.DeleteObject(a_Entity);
}

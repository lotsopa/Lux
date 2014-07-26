#include "LuxPCH.h"
#include "LuxEntity.h"

Lux::Entity::Entity() : m_Index(-1)
{
	Reset();
}

Lux::Entity::~Entity()
{

}

//Lux::Component* Lux::Entity::GetComponent(ComponentTypes a_Type)
//{
//	return m_Components[a_Type];
//}

void Lux::Entity::Reset()
{
	//for (unsigned int i = 0; i < COMPONENTS_SIZE; i++)
	//{
	//	m_Components[i] = nullptr;
	//}
}

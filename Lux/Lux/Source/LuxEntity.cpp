#include "LuxPCH.h"
#include "LuxMesh.h"
#include "LuxEntity.h"

Lux::Entity::Entity(unsigned int a_NumMeshes) : m_NumMeshes(a_NumMeshes)
{
	m_Meshes = new Mesh*[m_NumMeshes];
}

Lux::Entity::~Entity()
{
	for (unsigned int i = 0; i < m_NumMeshes; i++)
	{
		SafePtrDelete(m_Meshes[i]);
	}
	SafeArrayDelete(m_Meshes);
}

void Lux::Entity::AddMesh(Mesh* a_Mesh)
{
	static unsigned int ctr = 0;

	if (ctr >= m_NumMeshes)
	{
		ThrowError("Could not insert mesh in entity. Mesh index too high.");
	}

	m_Meshes[ctr] = a_Mesh;
	ctr++;
}

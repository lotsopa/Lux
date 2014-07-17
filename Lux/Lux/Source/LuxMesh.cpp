#include "LuxPCH.h"
#include "LuxSubMesh.h"
#include "LuxMeshAnimation.h"
#include "LuxMesh.h"

Lux::Mesh::Mesh(unsigned int a_NumMeshes, unsigned int a_NumAnims) : m_NumMeshes(a_NumMeshes), m_AnimationData(nullptr), m_NumAnimations(a_NumAnims)
{
	if (m_NumMeshes)
	{
		m_Meshes = new SubMesh*[m_NumMeshes];
	}

	if (m_NumAnimations)
	{
		m_AnimationData = new MeshAnimation*[m_NumAnimations];
	}
}

Lux::Mesh::~Mesh()
{
	for (unsigned int i = 0; i < m_NumMeshes; i++)
	{
		SafePtrDelete(m_Meshes[i]);
	}
	SafeArrayDelete(m_Meshes);

	for (unsigned int i = 0; i < m_NumAnimations; i++)
	{
		SafePtrDelete(m_AnimationData[i]);
	}
	SafeArrayDelete(m_AnimationData);
}

void Lux::Mesh::AddMesh(SubMesh* a_Mesh)
{
	LuxAssert(a_Mesh);
	static unsigned int ctr = 0;

	if (ctr >= m_NumMeshes)
	{
		ThrowError("Could not insert mesh in entity. Mesh index too high.");
	}

	m_Meshes[ctr] = a_Mesh;
	ctr++;
}

void Lux::Mesh::AddAnimation(MeshAnimation* a_Anim)
{
	LuxAssert(a_Anim);
	static unsigned int ctr = 0;

	if (ctr >= m_NumMeshes)
	{
		ThrowError("Could not insert animation in entity. Animation index too high.");
	}

	m_AnimationData[ctr] = a_Anim;
	ctr++;
}

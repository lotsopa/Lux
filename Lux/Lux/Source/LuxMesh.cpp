#include "LuxPCH.h"
#include "LuxSubMesh.h"
#include "LuxMeshAnimation.h"
#include "LuxMesh.h"

Lux::Mesh::Mesh(unsigned int a_NumMeshes, unsigned int a_NumAnims) :
Component(),
m_NumSubMeshes(a_NumMeshes), 
m_AnimationData(nullptr), 
m_NumAnimations(a_NumAnims)
{
	if (m_NumSubMeshes)
	{
		m_SubMeshes = new SubMesh*[m_NumSubMeshes];
	}

	if (m_NumAnimations)
	{
		m_AnimationData = new MeshAnimation*[m_NumAnimations];
	}
}

Lux::Mesh::Mesh() :
m_NumSubMeshes(0),
m_AnimationData(nullptr),
m_SubMeshes(nullptr),
m_NumAnimations(0)
{

}

Lux::Mesh::Mesh(const Mesh& a_Mesh)
{
	m_NumAnimations = a_Mesh.m_NumAnimations;
	m_NumSubMeshes = a_Mesh.m_NumSubMeshes;

	if (m_NumSubMeshes)
	{
		m_SubMeshes = new SubMesh*[m_NumSubMeshes];

		for (unsigned int i = 0; i < m_NumSubMeshes; i++)
		{
			m_SubMeshes[i] = new SubMesh(*a_Mesh.m_SubMeshes[i]);
		}
	}

	if (m_NumAnimations)
	{
		m_AnimationData = new MeshAnimation*[m_NumAnimations];

		for (unsigned int i = 0; i < m_NumAnimations; i++)
		{
			m_AnimationData[i] = new MeshAnimation(*a_Mesh.m_AnimationData[i]);
		}
	}
}

Lux::Mesh::~Mesh()
{
	Reset();
}

void Lux::Mesh::AddSubMesh(SubMesh* a_Mesh)
{
	LuxAssert(a_Mesh);
	static unsigned int ctr = 0;

	if (ctr >= m_NumSubMeshes)
	{
		ThrowError("Could not insert mesh in entity. Mesh index too high.");
	}

	m_SubMeshes[ctr] = a_Mesh;
	ctr++;
}

void Lux::Mesh::AddAnimation(MeshAnimation* a_Anim)
{
	LuxAssert(a_Anim);
	static unsigned int ctr = 0;

	if (ctr >= m_NumSubMeshes)
	{
		ThrowError("Could not insert animation in entity. Animation index too high.");
	}

	m_AnimationData[ctr] = a_Anim;
	ctr++;
}

void Lux::Mesh::Reset()
{
	for (unsigned int i = 0; i < m_NumSubMeshes; i++)
	{
		SafePtrDelete(m_SubMeshes[i]);
	}
	SafeArrayDelete(m_SubMeshes);

	for (unsigned int i = 0; i < m_NumAnimations; i++)
	{
		SafePtrDelete(m_AnimationData[i]);
	}
	SafeArrayDelete(m_AnimationData);

	m_NumSubMeshes = 0;
	m_NumAnimations = 0;
}

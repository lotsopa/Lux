#include "LuxPCH.h"
#include "LuxBufferOGL.h"
#include "LuxVertexArrayOGL.h"
#include "LuxSubMesh.h"
#include "LuxMeshAnimation.h"
#include "LuxMesh.h"
#include "LuxKey.h"
#include "LuxShader.h"
#include "LuxShaderOGL.h"
#include "LuxSubMeshOGL.h"
#include "LuxMeshOGL.h"

Lux::Core::Internal::MeshOGL::MeshOGL() : 
m_NumSubMeshes(0),
m_AnimationData(nullptr),
m_SubMeshes(nullptr),
m_NumAnimations(0)
{

}

Lux::Core::Internal::MeshOGL::MeshOGL(unsigned int a_NumMeshes, unsigned int a_NumAnims) :
m_NumSubMeshes(a_NumMeshes),
m_AnimationData(nullptr),
m_NumAnimations(a_NumAnims)
{
	if (m_NumSubMeshes)
	{
		m_SubMeshes = new SubMeshOGL*[m_NumSubMeshes];
	}

	if (m_NumAnimations)
	{
		m_AnimationData = new MeshAnimation*[m_NumAnimations];
	}
}


Lux::Core::Internal::MeshOGL::~MeshOGL()
{
	for (unsigned int i = 0; i < m_NumSubMeshes; i++)
	{
		Utility::SafePtrDelete(m_SubMeshes[i]);
	}
	Utility::SafeArrayDelete(m_SubMeshes);

	for (unsigned int i = 0; i < m_NumAnimations; i++)
	{
		Utility::SafePtrDelete(m_AnimationData[i]);
	}
	Utility::SafeArrayDelete(m_AnimationData);

	m_NumSubMeshes = 0;
	m_NumAnimations = 0;
}

void Lux::Core::Internal::MeshOGL::AddSubMesh(SubMesh* a_Mesh)
{
	LuxAssert(a_Mesh);
	static unsigned int ctr = 0;

	if (ctr >= m_NumSubMeshes)
	{
		Utility::ThrowError("Could not insert mesh in entity. Mesh index too high.");
	}

	m_SubMeshes[ctr] = (SubMeshOGL*)a_Mesh;
	ctr++;
}

void Lux::Core::Internal::MeshOGL::AddAnimation(MeshAnimation* a_Anim)
{
	LuxAssert(a_Anim);
	static unsigned int ctr = 0;

	if (ctr >= m_NumSubMeshes)
	{
		Utility::ThrowError("Could not insert animation in entity. Animation index too high.");
	}

	m_AnimationData[ctr] = a_Anim;
	ctr++;
}

void Lux::Core::Internal::MeshOGL::ConnectWithShader(Shader* a_Shader)
{
	for (unsigned int i = 0; i < m_NumSubMeshes; i++)
	{
		m_SubMeshes[i]->ConnectWithShader((ShaderOGL*)a_Shader);
	}
}

const unsigned int Lux::Core::Internal::MeshOGL::GetNumSubMeshes()
{
	return m_NumSubMeshes;
}

Lux::Core::SubMesh* Lux::Core::Internal::MeshOGL::GetSubMesh(const unsigned int a_Index)
{
	return m_SubMeshes[a_Index];
}

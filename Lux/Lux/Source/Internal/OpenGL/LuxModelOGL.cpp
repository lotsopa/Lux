#include "LuxPCH.h"
#include "LuxBufferOGL.h"
#include "LuxVertexArrayOGL.h"
#include "LuxMesh.h"
#include "LuxMeshAnimation.h"
#include "LuxModel.h"
#include "LuxKey.h"
#include "LuxShader.h"
#include "LuxShaderOGL.h"
#include "LuxMeshOGL.h"
#include "LuxModelOGL.h"

Lux::Core::Internal::ModelOGL::ModelOGL() : 
m_NumMeshes(0),
m_AnimationData(nullptr),
m_Meshes(nullptr),
m_NumAnimations(0),
m_AnimCtr(0),
m_MeshCtr(0)
{

}

Lux::Core::Internal::ModelOGL::ModelOGL(unsigned int a_NumMeshes, unsigned int a_NumAnims) :
m_NumMeshes(a_NumMeshes),
m_AnimationData(nullptr),
m_NumAnimations(a_NumAnims),
m_AnimCtr(0),
m_MeshCtr(0)
{
	if (m_NumMeshes)
	{
		m_Meshes = new MeshOGL*[m_NumMeshes];
	}

	if (m_NumAnimations)
	{
		m_AnimationData = new MeshAnimation*[m_NumAnimations];
	}
}


Lux::Core::Internal::ModelOGL::~ModelOGL()
{
	for (unsigned int i = 0; i < m_NumMeshes; i++)
	{
		Utility::SafePtrDelete(m_Meshes[i]);
	}
	Utility::SafeArrayDelete(m_Meshes);

	for (unsigned int i = 0; i < m_NumAnimations; i++)
	{
		Utility::SafePtrDelete(m_AnimationData[i]);
	}
	Utility::SafeArrayDelete(m_AnimationData);

	m_NumMeshes = 0;
	m_NumAnimations = 0;
}

void Lux::Core::Internal::ModelOGL::AddMesh(Mesh* a_Mesh)
{
	LuxAssert(a_Mesh);

	if (m_MeshCtr >= m_NumMeshes)
	{
		Utility::ThrowError("Could not insert mesh in entity. Mesh index too high.");
	}

	m_Meshes[m_MeshCtr] = (MeshOGL*)a_Mesh;
	m_MeshCtr++;
}

void Lux::Core::Internal::ModelOGL::AddAnimation(MeshAnimation* a_Anim)
{
	LuxAssert(a_Anim);

	if (m_AnimCtr >= m_NumMeshes)
	{
		Utility::ThrowError("Could not insert animation in entity. Animation index too high.");
	}

	m_AnimationData[m_AnimCtr] = a_Anim;
	m_AnimCtr++;
}

const unsigned int Lux::Core::Internal::ModelOGL::GetNumMeshes()
{
	return m_NumMeshes;
}

Lux::Core::ObserverPtr<Lux::Core::Mesh> Lux::Core::Internal::ModelOGL::GetMesh(const unsigned int a_Index)
{
	return ObserverPtr<Mesh>(m_Meshes[a_Index]);
}

void Lux::Core::Internal::ModelOGL::SetMaterialProperties(Core::ObserverPtr<Material>& a_Mat)
{
	for (unsigned int i = 0; i < m_NumMeshes; i++)
	{
		m_Meshes[i]->SetMaterialProperties(a_Mat);
	}
}

void Lux::Core::Internal::ModelOGL::SetShader(ObserverPtr<Shader>& a_Shader)
{
	for (unsigned int i = 0; i < m_NumMeshes; i++)
	{
		m_Meshes[i]->SetShader(a_Shader);
	}
}

void Lux::Core::Internal::ModelOGL::SetTexture(TextureIndex a_Idx, Core::ObserverPtr<Texture2D>& a_Tex)
{
	for (unsigned int i = 0; i < m_NumMeshes; i++)
	{
		m_Meshes[i]->SetTexture(a_Idx, a_Tex);
	}
}

#include "LuxPCH.h"
#include "LuxMesh.h"
#include "LuxMeshRenderer.h"

Lux::MeshRenderer::MeshRenderer() : Component(), m_Mesh(nullptr)
{

}

Lux::MeshRenderer::MeshRenderer(Mesh* a_Mesh) : Component(), m_Mesh(a_Mesh)
{

}

Lux::MeshRenderer::~MeshRenderer()
{

}

void Lux::MeshRenderer::Reset()
{

}

void Lux::MeshRenderer::SetMesh(Mesh* a_Mesh)
{
	m_Mesh = a_Mesh;
}

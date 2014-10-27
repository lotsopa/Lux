#include "LuxPCH.h"
#include "LuxMesh.h"
#include "LuxMeshRenderer.h"

Lux::Graphics::MeshRenderer::MeshRenderer() : Component(), m_Mesh(nullptr)
{

}

Lux::Graphics::MeshRenderer::MeshRenderer(Core::Mesh* a_Mesh) : Component(), m_Mesh(a_Mesh)
{

}

Lux::Graphics::MeshRenderer::~MeshRenderer()
{

}

void Lux::Graphics::MeshRenderer::Reset()
{
	m_Mesh = nullptr;
}

void Lux::Graphics::MeshRenderer::SetMesh(Core::Mesh* a_Mesh)
{
	m_Mesh = a_Mesh;
}

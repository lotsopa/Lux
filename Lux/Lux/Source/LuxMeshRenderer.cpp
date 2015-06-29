#include "LuxPCH.h"
#include "LuxMesh.h"
#include "LuxMeshRenderer.h"

Lux::Graphics::MeshRenderer::MeshRenderer() : Component()
{

}

Lux::Graphics::MeshRenderer::MeshRenderer(const MeshRenderer& a_Other)
{
	m_Mesh.reset(a_Other.m_Mesh.get());
	m_Index = a_Other.m_Index;
}

Lux::Graphics::MeshRenderer::~MeshRenderer()
{

}

void Lux::Graphics::MeshRenderer::Reset()
{
	m_Mesh.release();
}

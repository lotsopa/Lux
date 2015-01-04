#include "LuxPCH.h"
#include "LuxVertex.h"
#include "LuxSubMesh.h"
#include "LuxSubMeshDX11.h"
#include "LuxKey.h"
#include "LuxShader.h"

Lux::Core::Internal::SubMeshDX11::SubMeshDX11() : SubMesh()
{

}

Lux::Core::Internal::SubMeshDX11::SubMeshDX11(const SubMeshDX11& a_SubMesh) :SubMesh(a_SubMesh)
{

}

Lux::Core::Internal::SubMeshDX11::SubMeshDX11(aiMesh& a_Mesh) : SubMesh(a_Mesh)
{

}

Lux::Core::Internal::SubMeshDX11::~SubMeshDX11()
{

}

void Lux::Core::Internal::SubMeshDX11::PreRender()
{

}

void Lux::Core::Internal::SubMeshDX11::PostRender()
{

}

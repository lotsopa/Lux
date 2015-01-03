#include "LuxPCH.h"
#include "LuxKey.h"
#include "LuxMaterial.h"
#include "LuxSubMesh.h"
#include "LuxMesh.h"
#include "LuxMeshAnimation.h"
#include "LuxResourceHandler.h"
#include "LuxResourceHandlerDX11.h"
#include "LuxTexture.h"
#include "LuxShader.h"
#include "LuxFileHandler.h"
#include "LuxShaderFileParser.h"

#ifndef YY_NO_UNISTD_H
#define YY_NO_UNISTD_H
#endif
extern "C"
{
#include "LuxFileScanner.h"
}

// TODO
Lux::Core::Internal::ResourceHandlerDX11::~ResourceHandlerDX11()
{

}

Lux::Core::Mesh* Lux::Core::Internal::ResourceHandlerDX11::CreateMeshFromFile(const String& a_File, const String& a_EntityName, unsigned int a_PostProcessFlags)
{
	return nullptr;
}

Lux::Core::Mesh* Lux::Core::Internal::ResourceHandlerDX11::CreateMeshFromMemory(FileInfo* a_Info, const String& a_EntityName, unsigned int a_PostProcessFlags)
{
	return nullptr;
}

Lux::Core::Texture* Lux::Core::Internal::ResourceHandlerDX11::CreateTextureFromFile(const String& a_File, const String& a_TexName)
{
	return nullptr;
}

Lux::Core::Texture* Lux::Core::Internal::ResourceHandlerDX11::CreateTextureFromMemory(FileInfo* a_Info, const String& a_TexName)
{
	return nullptr;
}

Lux::Core::Shader* Lux::Core::Internal::ResourceHandlerDX11::CreateShaderFromFile(const String& a_File, const String& a_ShaderName)
{
	return nullptr;
}

Lux::Core::Material* Lux::Core::Internal::ResourceHandlerDX11::CreateMaterial(const String& a_Name)
{
	return nullptr;
}

Lux::Core::Mesh* Lux::Core::Internal::ResourceHandlerDX11::GetMesh(const String& a_Name)
{
	return nullptr;
}

Lux::Core::Shader* Lux::Core::Internal::ResourceHandlerDX11::GetShader(const String& a_Name)
{
	return nullptr;
}

Lux::Core::Material* Lux::Core::Internal::ResourceHandlerDX11::GetMaterial(const String& a_Name)
{
	return nullptr;
}

Lux::Core::Texture* Lux::Core::Internal::ResourceHandlerDX11::GetTexture(const String& a_Name)
{
	return nullptr;
}

bool Lux::Core::Internal::ResourceHandlerDX11::MaterialExists(const String& a_Name)
{
	return false;
}

bool Lux::Core::Internal::ResourceHandlerDX11::MeshExists(const String& a_Name)
{
	return false;
}

bool Lux::Core::Internal::ResourceHandlerDX11::ShaderExists(const String& a_Name)
{
	return false;
}

bool Lux::Core::Internal::ResourceHandlerDX11::TextureExists(const String& a_Name)
{
	return false;
}

bool Lux::Core::Internal::ResourceHandlerDX11::DeleteTexture(const String& a_Name)
{
	return false;
}

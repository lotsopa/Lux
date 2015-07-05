#include "LuxPCH.h"
#include "LuxMaterialShaderResource.h"

Lux::Graphics::MaterialShaderResource::MaterialShaderResource()
{

}

Lux::Graphics::MaterialShaderResource::MaterialShaderResource(const Core::Material& a_MatRes)
{
	m_DiffuseColor = a_MatRes.GetDiffuseColor();
	m_SpecularColor = a_MatRes.GetSpecularColor();
	m_Specular = a_MatRes.GetShininess();
	m_AmbientColor = a_MatRes.GetAmbientColor();
}

Lux::Graphics::MaterialShaderResource::~MaterialShaderResource()
{

}

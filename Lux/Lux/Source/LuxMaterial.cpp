#include "LuxPCH.h"
#include "LuxMaterialResource.h"
#include "LuxComponent.h"
#include "LuxMaterial.h"
#include "LuxTexture.h"
#include "LuxTexture2D.h"
#include "LuxKey.h"
#include "LuxShader.h"

Lux::Graphics::Material::Material() 
{
	Reset();
}

Lux::Graphics::Material::~Material()
{

}

void Lux::Graphics::Material::Reset()
{
	m_MaterialProperties = nullptr;
	m_DiffuseTexture = nullptr;
	m_Shader = nullptr;
}

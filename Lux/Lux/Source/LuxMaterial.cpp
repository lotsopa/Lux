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

Lux::Graphics::Material::Material(const Material& a_Other)
{
	m_Index = a_Other.m_Index;
	m_MaterialProperties.reset(a_Other.m_MaterialProperties.get());
	m_DiffuseTexture.reset(a_Other.m_DiffuseTexture.get());
	m_Shader.reset(a_Other.m_Shader.get());;
}

Lux::Graphics::Material::~Material()
{

}

void Lux::Graphics::Material::Reset()
{
	m_MaterialProperties.release();
	m_DiffuseTexture.release();
	m_Shader.release();
}

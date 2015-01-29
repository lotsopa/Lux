#include "LuxPCH.h"
#include "LuxMaterial.h"
#include "LuxComponent.h"
#include "LuxMaterialComponent.h"
#include "LuxTexture.h"
#include "LuxTexture2D.h"
#include "LuxKey.h"
#include "LuxShader.h"

Lux::Graphics::MaterialComponent::MaterialComponent() 
{
	Reset();
}

Lux::Graphics::MaterialComponent::~MaterialComponent()
{

}

void Lux::Graphics::MaterialComponent::Reset()
{
	m_MaterialProperties = nullptr;
	m_DiffuseTexture = nullptr;
	m_Shader = nullptr;
}

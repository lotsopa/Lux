#include "LuxPCH.h"
#include "LuxMaterial.h"
#include "LuxComponent.h"
#include "LuxMaterialComponent.h"

Lux::Graphics::MaterialComponent::MaterialComponent() : m_Material(nullptr)
{

}

Lux::Graphics::MaterialComponent::~MaterialComponent()
{

}

void Lux::Graphics::MaterialComponent::Reset()
{
	m_Material = nullptr;
}

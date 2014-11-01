#include "LuxPCH.h"
#include "LuxKey.h"
#include "LuxShader.h"
#include "LuxShaderComponent.h"

Lux::Graphics::ShaderComponent::ShaderComponent() : Component(), m_Shader(nullptr)
{

}

Lux::Graphics::ShaderComponent::ShaderComponent(Core::Shader* a_Shader) : Component(), m_Shader(a_Shader)
{

}

Lux::Graphics::ShaderComponent::~ShaderComponent()
{

}

void Lux::Graphics::ShaderComponent::Reset()
{
	m_Shader = nullptr;
}

void Lux::Graphics::ShaderComponent::SetShader(Core::Shader* a_Shader)
{
	m_Shader = a_Shader;
}

#include "LuxPCH.h"
#include "LuxTextureSampler.h"
#include "LuxTextureSamplerDX11.h"

Lux::Core::Internal::TextureSamplerDX11::TextureSamplerDX11(ID3D11DeviceContext* a_Context, TextureSamplerOptions& a_Options) : TextureSampler(a_Options), m_DeviceContext(a_Context)
{
	D3D11_SAMPLER_DESC desc;
	desc.AddressU = (D3D11_TEXTURE_ADDRESS_MODE)m_Options.m_AddressingU;
	desc.AddressV = (D3D11_TEXTURE_ADDRESS_MODE)m_Options.m_AddressingV;
	desc.AddressW = (D3D11_TEXTURE_ADDRESS_MODE)m_Options.m_AddressingW;
	desc.BorderColor[0] = m_Options.m_BorderColor.x;
	desc.BorderColor[1] = m_Options.m_BorderColor.y;
	desc.BorderColor[2] = m_Options.m_BorderColor.z;
	desc.BorderColor[3] = m_Options.m_BorderColor.w;
	desc.ComparisonFunc = (D3D11_COMPARISON_FUNC)m_Options.m_ComparisonFunc;
	desc.Filter = (D3D11_FILTER)m_Options.m_Filtering;
	desc.MaxAnisotropy = m_Options.m_MaxAnisotropy;
	desc.MaxLOD = m_Options.m_MaxLOD;
	desc.MinLOD = m_Options.m_MinLOD;
	desc.MipLODBias = m_Options.m_MipLODBias;

	HRESULT result;
	ID3D11Device* device = nullptr;
	m_DeviceContext->GetDevice(&device);
	result = device->CreateSamplerState(&desc, m_SamplerState.GetAddressOf());

	if (FAILED(result))
		Utility::ThrowError("Could not create DirectX 11 Sampler State");
}

Lux::Core::Internal::TextureSamplerDX11::~TextureSamplerDX11()
{
	m_SamplerState.Reset();
}

void Lux::Core::Internal::TextureSamplerDX11::Activate(unsigned int a_Unit, ShaderProgram a_Program)
{
	switch (a_Program)
	{
	case Lux::VERTEX_PROGRAM:
		m_DeviceContext->VSSetSamplers(a_Unit, 1, m_SamplerState.GetAddressOf());
		break;
	case Lux::FRAGMENT_PROGRAM:
		m_DeviceContext->PSSetSamplers(a_Unit, 1, m_SamplerState.GetAddressOf());
		break;
	case Lux::GEOMETRY_PROGRAM:
		m_DeviceContext->GSSetSamplers(a_Unit, 1, m_SamplerState.GetAddressOf());
		break;
	default:
		Utility::ThrowError("Could not activate texture sampler. Invalid Program.");
		break;
	}
}

void Lux::Core::Internal::TextureSamplerDX11::Deactivate()
{

}

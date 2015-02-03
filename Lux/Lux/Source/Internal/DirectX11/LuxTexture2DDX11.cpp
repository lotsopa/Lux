#include "LuxPCH.h"
#include "LuxTexture.h"
#include "LuxTexture2D.h"
#include "LuxTexture2DDX11.h"

// TODO
Lux::Core::Internal::Texture2DDX11::Texture2DDX11(ID3D11DeviceContext* a_Context, unsigned int a_ImgWidth, unsigned int a_ImgHeight, unsigned char* a_Bits) :
m_DeviceContext(a_Context)
{
	ID3D11Device* device = nullptr;
	m_DeviceContext->GetDevice(&device);

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = a_ImgWidth;
	desc.Height = a_ImgHeight;
	desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	
	// Create a texture and a resource view
	device->CreateTexture2D(&desc, nullptr, m_Texture2D.GetAddressOf());
	device->CreateShaderResourceView(m_Texture2D.Get(), 0, m_TextureResource.GetAddressOf());


	D3D11_MAPPED_SUBRESOURCE mapped;
	HRESULT result;
	int size = sizeof(unsigned char) * a_ImgWidth * a_ImgHeight * 4;
	result = m_DeviceContext->Map(m_Texture2D.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);

	if (FAILED(result))
		Utility::ThrowError("Error mapping texture buffer.");

	memcpy(mapped.pData, a_Bits, size);
	m_DeviceContext->Unmap(m_Texture2D.Get(), 0);
}

Lux::Core::Internal::Texture2DDX11::~Texture2DDX11()
{
	m_TextureResource.Reset();
	m_Texture2D.Reset();
}

void Lux::Core::Internal::Texture2DDX11::Bind(unsigned int a_Slot, const Key& a_Name, Shader* a_Shader, ShaderProgram a_Program)
{
	switch (a_Program)
	{
	case Lux::VERTEX_PROGRAM:
		m_DeviceContext->VSSetShaderResources(a_Slot, 1, m_TextureResource.GetAddressOf());
		break;
	case Lux::FRAGMENT_PROGRAM:
		m_DeviceContext->PSSetShaderResources(a_Slot, 1, m_TextureResource.GetAddressOf());
		break;
	case Lux::GEOMETRY_PROGRAM:
		m_DeviceContext->GSSetShaderResources(a_Slot, 1, m_TextureResource.GetAddressOf());
		break;
	default:
		Utility::ThrowError("Could not bind texture. Invalid shader enum.");
		break;
	}
}

void Lux::Core::Internal::Texture2DDX11::Unbind()
{

}

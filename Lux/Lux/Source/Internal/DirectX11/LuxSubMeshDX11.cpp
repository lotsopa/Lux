#include "LuxPCH.h"
#include "LuxVertex.h"
#include "LuxSubMesh.h"
#include "LuxSubMeshDX11.h"
#include "LuxKey.h"
#include "LuxShader.h"

const D3D11_INPUT_ELEMENT_DESC Lux::Core::Internal::SubMeshDX11::m_VertexInputDesc[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

// Helper for creating a D3D vertex or index buffer.
template<typename T>
static void CreateBuffer(_In_ ID3D11Device* device, T* data, unsigned int dataSize, D3D11_BIND_FLAG bindFlags, _Outptr_ ID3D11Buffer** pBuffer)
{
	assert(pBuffer != 0);

	D3D11_BUFFER_DESC bufferDesc = { 0 };

	bufferDesc.ByteWidth = dataSize * sizeof(T);
	bufferDesc.BindFlags = bindFlags;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA dataDesc = { 0 };

	dataDesc.pSysMem = data;

	HRESULT hr = device->CreateBuffer(&bufferDesc, &dataDesc, pBuffer);
	if (FAILED(hr))
	{
		Lux::Utility::ThrowError("Failed to create DX11 Buffer.");
	}
}

Lux::Core::Internal::SubMeshDX11::SubMeshDX11(ID3D11DeviceContext* a_DeviceContext) : SubMesh(), m_DeviceContext(a_DeviceContext)
{
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	m_DeviceContext->GetDevice(&device);
	CreateBuffer(device.Get(), m_Vertices, m_NumVertices, D3D11_BIND_VERTEX_BUFFER, &m_VertexBuffer);
	CreateBuffer(device.Get(), m_Indices, m_NumIndices, D3D11_BIND_INDEX_BUFFER, &m_IndexBuffer);
	SafeDeleteAttributes();
}

Lux::Core::Internal::SubMeshDX11::SubMeshDX11(const SubMeshDX11& a_SubMesh) : SubMesh(a_SubMesh)
{
	m_DeviceContext = a_SubMesh.m_DeviceContext;
	m_IndexBuffer = a_SubMesh.m_IndexBuffer;
	m_VertexBuffer = a_SubMesh.m_VertexBuffer;
	SafeDeleteAttributes();
}

Lux::Core::Internal::SubMeshDX11::SubMeshDX11(aiMesh& a_Mesh, ID3D11DeviceContext* a_DeviceContext) : SubMesh(a_Mesh), m_DeviceContext(a_DeviceContext)
{
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	m_DeviceContext->GetDevice(device.GetAddressOf());
	CreateBuffer(device.Get(), m_Vertices, m_NumVertices, D3D11_BIND_VERTEX_BUFFER, &m_VertexBuffer);
	CreateBuffer(device.Get(), m_Indices, m_NumIndices, D3D11_BIND_INDEX_BUFFER, &m_IndexBuffer);
	SafeDeleteAttributes();
}

Lux::Core::Internal::SubMeshDX11::~SubMeshDX11()
{
	m_VertexBuffer.Reset();
	m_IndexBuffer.Reset();
}

void Lux::Core::Internal::SubMeshDX11::PreRender()
{
	const UINT strides[] = { sizeof(Vertex) };
	const UINT offsets[] = { 0 };

	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_DeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), strides, offsets);
	m_DeviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void Lux::Core::Internal::SubMeshDX11::PostRender()
{

}

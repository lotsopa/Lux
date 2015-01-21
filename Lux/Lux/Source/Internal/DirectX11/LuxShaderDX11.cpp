#include "LuxPCH.h"
#include "LuxKey.h"
#include "LuxShader.h"
#include "LuxShaderFileParser.h"
#include "LuxShaderDX11.h"

Lux::Core::Internal::ShaderDX11::ShaderDX11(std::vector<DX11CompiledShader>& a_ShaderList, ID3D11DeviceContext* a_DeviceContext) : Shader(), m_DeviceContext(a_DeviceContext)
{
	for (unsigned int i = 0; i < a_ShaderList.size(); i++)
	{
		switch (a_ShaderList[i].m_Type)
		{
		case FRAGMENT_PROGRAM:
			m_BindShaderFuncList.push_back(std::bind(&ShaderDX11::SetShader<ID3D11PixelShader>, this, a_ShaderList[i].m_Shader));
			break;

		case VERTEX_PROGRAM:
			m_BindShaderFuncList.push_back(std::bind(&ShaderDX11::SetShader<ID3D11VertexShader>, this, a_ShaderList[i].m_Shader));
			break;

		case GEOMETRY_PROGRAM:
			m_BindShaderFuncList.push_back(std::bind(&ShaderDX11::SetShader<ID3D11GeometryShader>, this, a_ShaderList[i].m_Shader));
			break;

		default:
			Utility::ThrowError("Unknown shader type used when creating a DX11 shader.");
			break;
		}
	}
}

Lux::Core::Internal::ShaderDX11::~ShaderDX11()
{

}

void Lux::Core::Internal::ShaderDX11::Activate()
{
	for (unsigned int i = 0; i < m_BindShaderFuncList.size(); i++)
	{
		m_BindShaderFuncList[i];
	}
}

void Lux::Core::Internal::ShaderDX11::Deactivate()
{

}

void Lux::Core::Internal::ShaderDX11::Update()
{

}

void Lux::Core::Internal::ShaderDX11::BindUniformBuffer(const Key& a_Name, ShaderUniformBuffer& a_Buffer)
{

}

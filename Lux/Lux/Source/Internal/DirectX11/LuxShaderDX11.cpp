#include "LuxPCH.h"
#include "LuxKey.h"
#include "LuxShader.h"
#include "LuxShaderFileParser.h"
#include "LuxShaderDX11.h"

Lux::Core::Internal::ShaderDX11::ShaderDX11(std::vector<DX11CompiledShader>& a_ShaderList, ID3D11DeviceContext* a_DeviceContext) : Shader(), m_DeviceContext(a_DeviceContext)
{
	m_ShaderList = a_ShaderList;
	for (unsigned int i = 0; i < m_ShaderList.size(); i++)
	{
		switch (m_ShaderList[i].m_Type)
		{
		case FRAGMENT_PROGRAM:
			m_BindShaderFuncList.push_back(std::bind(&ShaderDX11::SetShader<ID3D11PixelShader>, this, m_ShaderList[i].m_Shader));
			break;

		case VERTEX_PROGRAM:
			m_BindShaderFuncList.push_back(std::bind(&ShaderDX11::SetShader<ID3D11VertexShader>, this, m_ShaderList[i].m_Shader));
			break;

		case GEOMETRY_PROGRAM:
			m_BindShaderFuncList.push_back(std::bind(&ShaderDX11::SetShader<ID3D11GeometryShader>, this, m_ShaderList[i].m_Shader));
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
		m_BindShaderFuncList[i](m_ShaderList[i].m_Shader);
	}
}

void Lux::Core::Internal::ShaderDX11::Deactivate()
{
	
}

void Lux::Core::Internal::ShaderDX11::Update()
{
	for (unsigned int i = 0; i < m_ConstantBuffers.size(); ++i)
	{
		std::queue<unsigned int>& modifiedVals = m_ConstantBuffers[i].m_Buffer.GetModifiedValues();
		if (!modifiedVals.empty())
		{
			// clear
			modifiedVals = std::queue<unsigned int>();
			m_ConstantBuffers[i].Update();
		}

		m_ConstantBuffers[i].Use();
	}
}

void Lux::Core::Internal::ShaderDX11::BindUniformBuffer(const Key& a_Name, ShaderUniformBuffer& a_Buffer, ShaderProgram a_Type)
{
	ID3D11ShaderReflection* shaderReflector = nullptr;
	unsigned int bufferSize = 0;
	unsigned int bufferIdx = D3D11_COMMONSHADER_CONSTANT_BUFFER_HW_SLOT_COUNT + 1;
	for (unsigned int i = 0; i < m_ShaderList.size(); i++)
	{
		if (m_ShaderList[i].m_Type == a_Type)
		{
			D3DReflect(m_ShaderList[i].m_CompileBlob->GetBufferPointer(), m_ShaderList[i].m_CompileBlob->GetBufferSize(),
				IID_ID3D11ShaderReflection, (void**)&shaderReflector);

			ID3D11ShaderReflectionConstantBuffer* cBuffer = shaderReflector->GetConstantBufferByName(a_Name.GetName().c_str());

			if (cBuffer == nullptr)
			{
				Utility::ThrowError("Invalid Uniform Buffer Name: " + a_Name.GetName());
			}
			D3D11_SHADER_BUFFER_DESC cBufferDesc;
			cBuffer->GetDesc(&cBufferDesc);

			for (unsigned int i = 0; i < D3D11_COMMONSHADER_CONSTANT_BUFFER_HW_SLOT_COUNT; i++)
			{
				ID3D11ShaderReflectionConstantBuffer* cBuffer2 = shaderReflector->GetConstantBufferByIndex(i);
				D3D11_SHADER_BUFFER_DESC cBufferDesc2;
				cBuffer2->GetDesc(&cBufferDesc2);
				if (cBufferDesc.Name == cBufferDesc2.Name)
				{
					bufferIdx = i;
					bufferSize = cBufferDesc.Size;
					break;
				}
			}

			// Should not happen
			if (bufferIdx == D3D11_COMMONSHADER_CONSTANT_BUFFER_HW_SLOT_COUNT + 1)
			{
				Utility::ThrowError("Could not retrieve uniform buffer index.");
			}
		}
	}

	m_ConstantBuffers.emplace(m_ConstantBuffers.end(), a_Buffer, bufferIdx, bufferSize, a_Type, m_DeviceContext);
}

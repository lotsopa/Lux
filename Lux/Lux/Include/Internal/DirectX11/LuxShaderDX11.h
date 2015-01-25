#ifndef LUX_SHADER_DX11_H
#define LUX_SHADER_DX11_H

namespace Lux
{
	namespace Core
	{
		namespace Internal
		{
			struct DX11CompiledShader
			{
				void* m_Shader;
				ID3DBlob* m_CompileBlob; // Used for reflection
				ShaderProgram m_Type;
			};
			class ShaderDX11 : public Shader
			{
			public:
				ShaderDX11(std::vector<DX11CompiledShader>& a_ShaderList, ID3D11DeviceContext* a_DeviceContext);
				virtual ~ShaderDX11();

				virtual void Activate();
				virtual void Deactivate();

				virtual void BindUniformBuffer(const Key& a_Name, ShaderUniformBuffer& a_Buffer, ShaderProgram a_Type);
				virtual void Update();

			private:
				// TODO
				typedef std::vector <std::function<void(void*)>> DX11ShaderFuncList;
				DX11ShaderFuncList m_BindShaderFuncList;
				ID3D11DeviceContext* m_DeviceContext;
				std::vector<DX11CompiledShader> m_ShaderList;

				struct UniformBufferEntry
				{
					UniformBufferEntry(ShaderUniformBuffer& a_UniformBuffer, const unsigned int a_BuffIndex, const unsigned int a_BuffSize, ShaderProgram a_Type, ID3D11DeviceContext* a_Context) :
						m_Buffer(a_UniformBuffer), m_BufferIndex(a_BuffIndex), m_BufferSize(a_BuffSize), m_DeviceContext(a_Context), m_BufferType(a_Type)
					{
						m_Buffer.AllocateRawData(m_BufferSize);
						// Fill in a buffer description.
						D3D11_BUFFER_DESC cbDesc;
						cbDesc.ByteWidth = m_BufferSize;
						cbDesc.Usage = D3D11_USAGE_DEFAULT;
						cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
						cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
						cbDesc.MiscFlags = 0;
						cbDesc.StructureByteStride = 0;

						// Fill in the sub-resource data.
						D3D11_SUBRESOURCE_DATA InitData;
						InitData.pSysMem = m_Buffer.GetRawValueArray(); 
						InitData.SysMemPitch = 0;
						InitData.SysMemSlicePitch = 0;

						// Create the buffer.
						HRESULT hr;
						ID3D11Device* device = nullptr;
						m_DeviceContext->GetDevice(&device);
						hr = device->CreateBuffer(&cbDesc, &InitData, &m_DXBuffer);

						if (FAILED(hr))
							Utility::ThrowError("Error Creating DX11 Uniform Buffer");
					}
					~UniformBufferEntry()
					{
						m_DXBuffer->Release();
					}

					UniformBufferEntry& operator=(const UniformBufferEntry& other) // copy assignment
					{
						this->m_DXBuffer = other.m_DXBuffer;
						this->m_Buffer = other.m_Buffer;
						this->m_BufferIndex = other.m_BufferIndex;
						this->m_DeviceContext = other.m_DeviceContext;
						this->m_BufferSize = other.m_BufferSize;
						return *this;
					}

					void Update()
					{
						HRESULT result;
						D3D11_MAPPED_SUBRESOURCE mappedResource;
						result = m_DeviceContext->Map(m_DXBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

						if (FAILED(result))
							Utility::ThrowError("Could not map DX11 constant buffer.");

						float* destPtr = (float*)mappedResource.pData;
						float* srcPtr = (float*)m_Buffer.GetRawValueArray();
						for (unsigned int i = 0; i < m_BufferSize; ++i)
						{
							destPtr[i] = srcPtr[i];
						}
						m_DeviceContext->Unmap(m_DXBuffer, 0);
					}

					void Use()
					{
						switch (m_BufferType)
						{
						case VERTEX_PROGRAM:
							m_DeviceContext->VSSetConstantBuffers(m_BufferIndex, 1, &m_DXBuffer);
							break;

						case FRAGMENT_PROGRAM:
							m_DeviceContext->PSSetConstantBuffers(m_BufferIndex, 1, &m_DXBuffer);
							break;

						case GEOMETRY_PROGRAM:
							m_DeviceContext->GSSetConstantBuffers(m_BufferIndex, 1, &m_DXBuffer);
							break;
						}
					}

					ID3D11Buffer* m_DXBuffer;
					unsigned int m_BufferIndex;
					unsigned int m_BufferSize;
					ShaderUniformBuffer& m_Buffer;
					ID3D11DeviceContext* m_DeviceContext;
					ShaderProgram m_BufferType;
				};
				std::vector<UniformBufferEntry> m_ConstantBuffers;
				template<class T>
				void SetShader(void* shader)
				{
					Utility::ThrowError("Setting shader function not implemented for this shader type.");
				}

				template<>
				void SetShader<ID3D11PixelShader>(void* shader)
				{
					ID3D11PixelShader* pixShader = (ID3D11PixelShader*)shader;
					m_DeviceContext->PSSetShader(pixShader, NULL, 0);
				}

				template<>
				void SetShader<ID3D11VertexShader>(void* shader)
				{
					ID3D11VertexShader* vertShader = (ID3D11VertexShader*)shader;
					m_DeviceContext->VSSetShader(vertShader, NULL, 0);
				}

				template<>
				void SetShader<ID3D11GeometryShader>(void* shader)
				{
					ID3D11GeometryShader* geomShader = (ID3D11GeometryShader*)shader;
					m_DeviceContext->GSSetShader(geomShader, NULL, 0);
				}
			};
		}
	}
}

#endif
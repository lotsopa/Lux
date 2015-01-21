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
				ShaderProgram m_Type;
			};
			class ShaderDX11 : public Shader
			{
			public:
				ShaderDX11(std::vector<DX11CompiledShader>& a_ShaderList, ID3D11DeviceContext* a_DeviceContext);
				virtual ~ShaderDX11();

				virtual void Activate();
				virtual void Deactivate();

				virtual void BindUniformBuffer(const Key& a_Name, ShaderUniformBuffer& a_Buffer);
				virtual void Update();

			private:
				// TODO
				typedef std::vector <std::function<void(void*)>> DX11ShaderFuncList;
				DX11ShaderFuncList m_BindShaderFuncList;
				ID3D11DeviceContext* m_DeviceContext;

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
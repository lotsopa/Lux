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
				ShaderDX11(std::vector<DX11CompiledShader>& a_ShaderList);
				virtual ~ShaderDX11();

				virtual void Activate();
				virtual void Deactivate();

				virtual void SetUniformVariable(const Key& a_Name, ShaderVariable& a_Var);
				virtual void Update();

			private:
				// TODO
			};
		}
	}
}

#endif
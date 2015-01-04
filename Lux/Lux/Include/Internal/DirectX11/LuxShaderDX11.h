#ifndef LUX_SHADER_DX11_H
#define LUX_SHADER_DX11_H

namespace Lux
{
	namespace Core
	{
		namespace Internal
		{
			class ShaderDX11 : public Shader
			{
			public:
				ShaderDX11();
				virtual ~ShaderDX11();

				virtual void Activate() = 0;
				virtual void Deactivate() = 0;

				virtual void SetUniformVariable(const Key& a_Name, ShaderVariable& a_Var) = 0;
				virtual void Update() = 0;
			};
		}
	}
}

#endif
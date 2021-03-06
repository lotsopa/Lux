#ifndef LUX_TEXTURE_1D_DX11_H
#define LUX_TEXTURE_1D_DX11_H

namespace Lux
{
	namespace Core
	{
		namespace Internal
		{
			class Texture1DDX11 : public Texture1D
			{
			public:
				Texture1DDX11(unsigned int a_Width, unsigned char* a_Data);
				~Texture1DDX11();

				virtual void Bind(unsigned int a_Slot, const Key& a_Name, Shader* a_Shader, ShaderProgram a_Program);
				virtual void Unbind();
			};
		}
	}
}
#endif
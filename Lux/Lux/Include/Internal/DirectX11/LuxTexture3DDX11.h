#ifndef LUX_TEXTURE_3D_DX11_H
#define LUX_TEXTURE_3D_DX11_H

namespace Lux
{
	namespace Core
	{
		namespace Internal
		{
			class Texture3DDX11 : public Texture3D
			{
			public:

				virtual void Bind(unsigned int a_Slot, const Key& a_Name, Shader* a_Shader, ShaderProgram a_Program);
				virtual void Unbind();
			};
		}
	}
}
#endif
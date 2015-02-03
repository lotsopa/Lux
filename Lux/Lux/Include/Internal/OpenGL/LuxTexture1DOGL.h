#ifndef LUX_TEXTURE_1D_OGL_H
#define LUX_TEXTURE_1D_OGL_H

namespace Lux
{
	namespace Core
	{
		namespace Internal
		{
			class Texture1DOGL : public Texture1D
			{
			public:
				Texture1DOGL(unsigned int a_Width, unsigned char* a_Data);
				virtual ~Texture1DOGL();

				virtual void Bind(unsigned int a_Slot, const Key& a_Name, Shader* a_Shader, ShaderProgram a_Program);
				virtual void Unbind();

			private:
				unsigned int m_TextureID;
			};
		}
	}
}

#endif
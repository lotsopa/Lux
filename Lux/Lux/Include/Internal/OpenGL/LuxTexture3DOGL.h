#ifndef LUX_TEXTURE_3D_OGL_H
#define LUX_TEXTURE_3D_OGL_H

namespace Lux
{
	namespace Core
	{
		namespace Internal
		{
			class Texture3DOGL : public Texture3D
			{
			public:
				Texture3DOGL();
				virtual ~Texture3DOGL();

				virtual void Bind(unsigned int a_Slot, const Key& a_Name, Shader* a_Shader, ShaderProgram a_Program);
				virtual void Unbind();

			private:
				unsigned int m_TextureID;
			};
		}
	}
}

#endif
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

			private:
				unsigned int m_TextureID;
			};
		}
	}
}

#endif
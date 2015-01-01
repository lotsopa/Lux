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
				Texture1DOGL();
				virtual ~Texture1DOGL();

			private:
				unsigned int m_TextureID;
			};
		}
	}
}

#endif
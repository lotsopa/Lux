#ifndef LUX_TEXTURE_2D_DX11_H
#define LUX_TEXTURE_2D_DX11_H

namespace Lux
{
	namespace Core
	{
		namespace Internal
		{
			class Texture2DDX11 : public Texture2D
			{
			public:
				Texture2DDX11(unsigned int a_ImgWidth, unsigned int a_ImgHeight, unsigned char* a_Bits);
				~Texture2DDX11();
			};
		}
	}
}
#endif
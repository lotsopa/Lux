#ifndef LUX_TEXTURE_H
#define LUX_TEXTURE_H
#include "LuxKey.h"
namespace Lux
{
	namespace Core
	{
		class Shader;
		class Texture
		{
		public:
			Texture();
			virtual ~Texture();

			virtual void Bind(unsigned int a_Slot, const Key& a_Name, Shader* a_Shader, ShaderProgram a_Program) = 0;
			virtual void Unbind() = 0;
		};
	}
}

#endif
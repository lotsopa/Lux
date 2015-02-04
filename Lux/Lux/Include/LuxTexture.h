#ifndef LUX_TEXTURE_H
#define LUX_TEXTURE_H
#include "LuxKey.h"
namespace Lux
{
	namespace Core
	{
		class Shader;
		class TextureSampler;
		class Texture
		{
		public:
			Texture();
			virtual ~Texture();

			virtual void Bind(unsigned int a_Slot, const Key& a_Name, Shader* a_Shader, ShaderProgram a_Program) = 0;
			virtual void Unbind() = 0;

			inline void SetSampler(TextureSampler* a_Sampler) { m_Sampler = a_Sampler; }
			inline TextureSampler* GetSampler() { return m_Sampler; }
		protected:
			TextureSampler* m_Sampler;
		};
	}
}

#endif
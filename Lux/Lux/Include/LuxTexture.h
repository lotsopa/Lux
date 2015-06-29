#ifndef LUX_TEXTURE_H
#define LUX_TEXTURE_H
#include "LuxKey.h"
#include "LuxObserverPtr.h"

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

			inline void SetSampler(Core::ObserverPtr<TextureSampler>& a_Sampler) { m_Sampler.reset(a_Sampler.get()); }
			inline Lux::Core::ObserverPtr<TextureSampler>& GetSampler() { return m_Sampler; }
		protected:
			ObserverPtr<TextureSampler> m_Sampler;
		};
	}
}

#endif
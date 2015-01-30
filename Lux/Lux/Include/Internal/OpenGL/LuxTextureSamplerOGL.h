#ifndef LUX_TEXTURE_SAMPLER_OGL_H
#define LUX_TEXTURE_SAMPLER_OGL_H

namespace Lux
{
	namespace Core
	{
		namespace Internal
		{
			class TextureSamplerOGL : public TextureSampler
			{
			public:
				TextureSamplerOGL();
				TextureSamplerOGL(TextureSamplerOptions& a_Options);
				virtual ~TextureSamplerOGL();

			private:
				GLuint m_SamplerState;
			};
		}
	}
}

#endif
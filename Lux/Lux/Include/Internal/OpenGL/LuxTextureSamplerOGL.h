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
				TextureSamplerOGL(TextureSamplerOptions& a_Options);
				virtual ~TextureSamplerOGL();
				virtual void Activate();
				virtual void Deactivate();
			private:
				GLuint m_SamplerState;
				void SetTextureWrap(TextureAddressMode mode, GLint axis);
				void SetCompareFunc(TextureComparisonFunc func);
				void SetFilterMode(TextureFilter filter);
			};
		}
	}
}

#endif
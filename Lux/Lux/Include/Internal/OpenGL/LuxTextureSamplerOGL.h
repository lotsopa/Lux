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
				virtual void Activate(unsigned int a_Unit, ShaderProgram a_Program);
				virtual void Deactivate();
			private:
				GLuint m_SamplerState;
				GLuint m_LastBoundUnit;
				void SetTextureWrap(TextureAddressMode mode, GLint axis);
				void SetCompareFunc(TextureComparisonFunc func);
				void SetFilterMode(TextureFilter filter);
			};
		}
	}
}

#endif
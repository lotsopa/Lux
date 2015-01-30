#include "LuxPCH.h"
#include "LuxTextureSampler.h"
#include "LuxTextureSamplerOGL.h"
#include "LuxErrorCheckOGL.h"

// TODO
Lux::Core::Internal::TextureSamplerOGL::TextureSamplerOGL() : TextureSampler()
{
	glGenSamplers(1, &m_SamplerState);
	Utility::Internal::CheckOGLError();
}

Lux::Core::Internal::TextureSamplerOGL::TextureSamplerOGL(TextureSamplerOptions& a_Options) : TextureSampler(a_Options)
{
	glGenSamplers(1, &m_SamplerState);
	Utility::Internal::CheckOGLError();
}

Lux::Core::Internal::TextureSamplerOGL::~TextureSamplerOGL()
{
	glDeleteSamplers(1, &m_SamplerState);
	Utility::Internal::CheckOGLError();
}

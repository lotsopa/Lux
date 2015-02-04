#include "LuxPCH.h"
#include "LuxTextureSampler.h"
#include "LuxTextureSamplerOGL.h"
#include "LuxErrorCheckOGL.h"

Lux::Core::Internal::TextureSamplerOGL::TextureSamplerOGL(TextureSamplerOptions& a_Options) : TextureSampler(a_Options)
{
	glGenSamplers(1, &m_SamplerState);
	Utility::Internal::CheckOGLError();

	// Set the parameters of the sampler
	SetFilterMode(m_Options.m_Filtering);
	Utility::Internal::CheckOGLError();

	SetTextureWrap(m_Options.m_AddressingU, GL_TEXTURE_WRAP_S);
	Utility::Internal::CheckOGLError();
	SetTextureWrap(m_Options.m_AddressingV, GL_TEXTURE_WRAP_T);
	Utility::Internal::CheckOGLError();
	SetTextureWrap(m_Options.m_AddressingW, GL_TEXTURE_WRAP_R);
	Utility::Internal::CheckOGLError();

	glSamplerParameterfv(m_SamplerState, GL_TEXTURE_BORDER_COLOR, value_ptr(m_Options.m_BorderColor));
	Utility::Internal::CheckOGLError();

	glSamplerParameterf(m_SamplerState, GL_TEXTURE_MIN_LOD, m_Options.m_MinLOD);
	Utility::Internal::CheckOGLError();

	glSamplerParameterf(m_SamplerState, GL_TEXTURE_MAX_LOD, m_Options.m_MaxLOD);
	Utility::Internal::CheckOGLError();

	glSamplerParameterf(m_SamplerState, GL_TEXTURE_LOD_BIAS, m_Options.m_MipLODBias);
	Utility::Internal::CheckOGLError();

	SetCompareFunc(m_Options.m_ComparisonFunc);

	glSamplerParameterf(m_SamplerState, GL_TEXTURE_MAX_ANISOTROPY_EXT, (float)m_Options.m_MaxAnisotropy);
	Utility::Internal::CheckOGLError();
}

Lux::Core::Internal::TextureSamplerOGL::~TextureSamplerOGL()
{
	glDeleteSamplers(1, &m_SamplerState);
	Utility::Internal::CheckOGLError();
}

void Lux::Core::Internal::TextureSamplerOGL::SetTextureWrap(TextureAddressMode mode, GLint axis)
{
	switch (mode)
	{
	case LUX_TEXTURE_ADDRESS_WRAP:
		glSamplerParameteri(m_SamplerState, axis, GL_REPEAT);
		break;

	case LUX_TEXTURE_ADDRESS_BORDER:
		glSamplerParameteri(m_SamplerState, axis, GL_CLAMP_TO_BORDER);
		break;

	case LUX_TEXTURE_ADDRESS_CLAMP:
		glSamplerParameteri(m_SamplerState, axis, GL_CLAMP_TO_EDGE);
		break;

	case LUX_TEXTURE_ADDRESS_MIRROR:
		glSamplerParameteri(m_SamplerState, axis, GL_MIRRORED_REPEAT);
		break;

	case LUX_TEXTURE_ADDRESS_MIRROR_ONCE:
		glSamplerParameteri(m_SamplerState, axis, GL_MIRROR_CLAMP_TO_EDGE);
		break;

	default:
		Utility::ThrowError("Texture sampler - invalid enum");
		break;
	}
}

void Lux::Core::Internal::TextureSamplerOGL::SetCompareFunc(TextureComparisonFunc func)
{
	switch (func)
	{
	case Lux::Core::LUX_COMPARISON_NEVER:
		glSamplerParameteri(m_SamplerState, GL_TEXTURE_COMPARE_FUNC, GL_NEVER);
		break;
	case Lux::Core::LUX_COMPARISON_LESS:
		glSamplerParameteri(m_SamplerState, GL_TEXTURE_COMPARE_FUNC, GL_LESS);
		break;
	case Lux::Core::LUX_COMPARISON_EQUAL:
		glSamplerParameteri(m_SamplerState, GL_TEXTURE_COMPARE_FUNC, GL_EQUAL);
		break;
	case Lux::Core::LUX_COMPARISON_LESS_EQUAL:
		glSamplerParameteri(m_SamplerState, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		break;
	case Lux::Core::LUX_COMPARISON_GREATER:
		glSamplerParameteri(m_SamplerState, GL_TEXTURE_COMPARE_FUNC, GL_GREATER);
		break;
	case Lux::Core::LUX_COMPARISON_NOT_EQUAL:
		glSamplerParameteri(m_SamplerState, GL_TEXTURE_COMPARE_FUNC, GL_NOTEQUAL);
		break;
	case Lux::Core::LUX_COMPARISON_GREATER_EQUAL:
		glSamplerParameteri(m_SamplerState, GL_TEXTURE_COMPARE_FUNC, GL_GEQUAL);
		break;
	case Lux::Core::LUX_COMPARISON_ALWAYS:
		glSamplerParameteri(m_SamplerState, GL_TEXTURE_COMPARE_FUNC, GL_ALWAYS);
		break;
	default:
		Utility::ThrowError("Texture sampler - invalid enum");
		break;
	}
}

void Lux::Core::Internal::TextureSamplerOGL::SetFilterMode(TextureFilter filter)
{
	switch (filter)
	{
	case Lux::Core::LUX_FILTER_MIN_MAG_MIP_POINT:
		glSamplerParameteri(m_SamplerState, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glSamplerParameteri(m_SamplerState, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		break;
	case Lux::Core::LUX_FILTER_MIN_MAG_POINT_MIP_LINEAR:
		glSamplerParameteri(m_SamplerState, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glSamplerParameteri(m_SamplerState, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		break;
	case Lux::Core::LUX_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT:
		glSamplerParameteri(m_SamplerState, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glSamplerParameteri(m_SamplerState, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		break;
	case Lux::Core::LUX_FILTER_MIN_POINT_MAG_MIP_LINEAR:
		glSamplerParameteri(m_SamplerState, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glSamplerParameteri(m_SamplerState, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		break;
	case Lux::Core::LUX_FILTER_MIN_LINEAR_MAG_MIP_POINT:
		glSamplerParameteri(m_SamplerState, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glSamplerParameteri(m_SamplerState, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		break;
	case Lux::Core::LUX_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
		glSamplerParameteri(m_SamplerState, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glSamplerParameteri(m_SamplerState, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		break;
	case Lux::Core::LUX_FILTER_MIN_MAG_LINEAR_MIP_POINT:
		glSamplerParameteri(m_SamplerState, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glSamplerParameteri(m_SamplerState, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		break;
	case Lux::Core::LUX_FILTER_MIN_MAG_MIP_LINEAR:
		glSamplerParameteri(m_SamplerState, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glSamplerParameteri(m_SamplerState, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		break;
	default:
		glSamplerParameteri(m_SamplerState, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glSamplerParameteri(m_SamplerState, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		break;
	}
}

void Lux::Core::Internal::TextureSamplerOGL::Activate(unsigned int a_Unit, ShaderProgram a_Program)
{
	m_LastBoundUnit = a_Unit;
	glBindSampler(a_Unit, m_SamplerState);
}

void Lux::Core::Internal::TextureSamplerOGL::Deactivate()
{
	glBindSampler(m_LastBoundUnit, 0);
}

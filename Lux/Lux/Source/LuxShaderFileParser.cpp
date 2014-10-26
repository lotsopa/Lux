#include "LuxPCH.h"
#include "LuxKey.h"
#include "LuxShaderFileParser.h"

#define VERTEX_PROGRAM_NAME "VertexProgram"
#define FRAGMENT_PROGRAM_NAME "FragmentProgram"
#define GEOMETRY_PROGRAM_NAME "GeometryProgram"

Lux::Core::ShaderFileParser::ShaderFileParser() : 
m_State(STATE_BASE), m_PlatformState(STATE_BASE)
{
	for (unsigned int i = 0; i < NUM_SHADER_PROGRAMS; i++)
	{
		m_GLSLPrograms[i] = "";
		m_HLSLPrograms[i] = "";
	}
}

Lux::Core::ShaderFileParser::~ShaderFileParser()
{

}

void Lux::Core::ShaderFileParser::ProcessIdentifier(Key& a_Key)
{
	std::string name = a_Key.GetName();

	if (name == "Shader")
	{
		if (m_State == STATE_BASE)
			m_State = STATE_SHADER;
		else
			Utility::ThrowError("Logical error while parsing shader file.");
	}
	else if (name == "GLSL")
	{
		if (m_State == STATE_SHADER)
		{
			m_PlatformState = STATE_GLSL;
			m_State = STATE_GLSL;
		}
		else
		{
			Utility::ThrowError("Syntax error while parsing shader file.");
		}
	}
	else if (name == "HLSL")
	{
		if (m_State == STATE_SHADER)
		{
			m_PlatformState = STATE_HLSL;
			m_State = STATE_HLSL;
		}
		else
		{
			Utility::ThrowError("Syntax error while parsing shader file.");
		}
	}
	else if (name == VERTEX_PROGRAM_NAME)
	{
		if (m_State == STATE_GLSL || m_State == STATE_HLSL)
		{
			m_State = STATE_VERTEX_PROGRAM;
		}
		else
		{
			Utility::ThrowError("Syntax error while parsing shader file.");
		}
	}
	else if (name == FRAGMENT_PROGRAM_NAME)
	{
		if (m_State == STATE_GLSL || m_State == STATE_HLSL)
		{
			m_State = STATE_FRAGMENT_PROGRAM;
		}
		else
		{
			Utility::ThrowError("Syntax error while parsing shader file.");
		}
	}
	else if (name == GEOMETRY_PROGRAM_NAME)
	{
		if (m_State == STATE_GLSL || m_State == STATE_HLSL)
		{
			m_State = STATE_GEOMETRY_PROGRAM;
		}
		else
		{
			Utility::ThrowError("Syntax error while parsing shader file.");
		}
	}
	else
	{
		Utility::ThrowError("Syntax error while parsing shader file.");
	}
}

void Lux::Core::ShaderFileParser::ProcessString(Key& a_Key)
{
	switch (m_State)
	{
	case STATE_FRAGMENT_PROGRAM:
		
		if (m_PlatformState == STATE_GLSL)
		{
			m_GLSLPrograms[FRAGMENT_PROGRAM] = a_Key.GetName();
		}
		else
		{
			m_HLSLPrograms[FRAGMENT_PROGRAM] = a_Key.GetName();
		}
		break;

	case STATE_VERTEX_PROGRAM:
		if (m_PlatformState == STATE_GLSL)
		{
			m_GLSLPrograms[VERTEX_PROGRAM] = a_Key.GetName();
		}
		else
		{
			m_HLSLPrograms[VERTEX_PROGRAM] = a_Key.GetName();
		}
		break;

	case STATE_GEOMETRY_PROGRAM:
		if (m_PlatformState == STATE_GLSL)
		{
			m_GLSLPrograms[GEOMETRY_PROGRAM] = a_Key.GetName();
		}
		else
		{
			m_HLSLPrograms[GEOMETRY_PROGRAM] = a_Key.GetName();
		}
		break;

	}
}

void Lux::Core::ShaderFileParser::ProcessOpenBrace()
{
	if (m_State != STATE_SHADER && m_State != STATE_GLSL && m_State != STATE_HLSL)
	{
		Utility::ThrowError("Syntax error while parsing shader file.");
	}
}

void Lux::Core::ShaderFileParser::ProcessCloseBrace()
{
	switch (m_State)
	{
	case STATE_GLSL:
	case STATE_HLSL:
		m_State = STATE_SHADER;
		break;

	case STATE_SHADER:
		m_State = STATE_BASE;
		break;
	}
}

void Lux::Core::ShaderFileParser::ProcessEquals()
{
	switch (m_State)
	{
	case STATE_VERTEX_PROGRAM:
	case STATE_FRAGMENT_PROGRAM:
	case STATE_GEOMETRY_PROGRAM:
		break;

	default:
		Utility::ThrowError("Syntax error while parsing shader file.");
		break;
	}
}

void Lux::Core::ShaderFileParser::ProcessSemiColon()
{
	switch (m_State)
	{
	case STATE_FRAGMENT_PROGRAM:
	case STATE_GEOMETRY_PROGRAM:
	case STATE_VERTEX_PROGRAM:
		m_State = m_PlatformState;
		break;

	default:
		Utility::ThrowError("Syntax error while parsing shader file.");
		break;
	}
}

const Lux::String& Lux::Core::ShaderFileParser::GetParsedProgramGLSL(ShaderProgram a_ProgramType)
{
	return m_GLSLPrograms[a_ProgramType];
}

const Lux::String& Lux::Core::ShaderFileParser::GetParsedProgramHLSL(ShaderProgram a_ProgramType)
{
	return m_HLSLPrograms[a_ProgramType];
}

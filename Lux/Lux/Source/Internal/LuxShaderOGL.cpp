#include "LuxPCH.h"
#include "LuxKey.h"
#include "LuxShader.h"
#include "LuxShaderOGL.h"
#include "LuxErrorCheckOGL.h"

#define UNIFORM_INVALID_LOCATION -1
Lux::Core::Internal::ShaderOGL::ShaderOGL(std::vector<unsigned int>& a_LoadedShaders)
{
	m_ShaderProgram = CreateShaderProgram(a_LoadedShaders);
	LuxAssert(m_ShaderProgram != 0);
}

Lux::Core::Internal::ShaderOGL::~ShaderOGL()
{

}

unsigned int Lux::Core::Internal::ShaderOGL::CreateShaderProgram(std::vector<unsigned int>& shaders)
{
	// Check if we have any loaded shaders
	if (shaders.empty())
	{
		return 0;
	}

	// Create a shader program.
	GLuint program = glCreateProgram();

	// Attach the appropriate shader objects.
	for (GLuint shader : shaders)
	{
		glAttachShader(program, shader);
	}

	// Link the program
	glLinkProgram(program);

	// Check the link status.
	GLint linkStatus;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
	if (linkStatus != GL_TRUE)
	{
		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* infoLog = new GLchar[logLength];

		glGetProgramInfoLog(program, logLength, NULL, infoLog);

#ifdef _WIN32
		OutputDebugString(infoLog);
#else
		std::cerr << infoLog << std::endl;
#endif

		std::string errStr("Error Compiling shader program.");
		errStr.append(infoLog);
		delete infoLog;
		Utility::ThrowError(errStr);
		return 0;
	}
	return program;
}

void Lux::Core::Internal::ShaderOGL::Activate()
{
	glUseProgram(m_ShaderProgram);
	Utility::Internal::CheckOGLError();
}

void Lux::Core::Internal::ShaderOGL::Deactivate()
{
	glUseProgram(0);
}

void Lux::Core::Internal::ShaderOGL::SetUniformFloat(const Key& a_Name, float a_Val)
{
	unsigned int location = GetUniformLocation(a_Name);
	glUniform1f(location, a_Val);
}

void Lux::Core::Internal::ShaderOGL::SetUniformInt(const Key& a_Name, int a_Val)
{
	unsigned int location = GetUniformLocation(a_Name);
	glUniform1i(location, a_Val);
}

void Lux::Core::Internal::ShaderOGL::SetUniformVec2(const Key& a_Name, const vec2& a_Vec)
{
	unsigned int location = GetUniformLocation(a_Name);
	glUniform2fv(location, 1, glm::value_ptr(a_Vec));
}

void Lux::Core::Internal::ShaderOGL::SetUniformVec3(const Key& a_Name, const vec3& a_Vec)
{
	unsigned int location = GetUniformLocation(a_Name);
	glUniform3fv(location, 1, glm::value_ptr(a_Vec));
}

void Lux::Core::Internal::ShaderOGL::SetUniformVec4(const Key& a_Name, const vec4 a_Vec)
{
	unsigned int location = GetUniformLocation(a_Name);
	glUniform4fv(location, 1, glm::value_ptr(a_Vec));
}

void Lux::Core::Internal::ShaderOGL::SetUniformMat3x3(const Key& a_Name, const mat3x3& a_Mat)
{
	unsigned int location = GetUniformLocation(a_Name);
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(a_Mat));
}

void Lux::Core::Internal::ShaderOGL::SetUniformMat4x4(const Key& a_Name, const mat4x4& a_Mat)
{
	unsigned int location = GetUniformLocation(a_Name);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(a_Mat));
}

unsigned int Lux::Core::Internal::ShaderOGL::GetUniformLocation(const Key& a_Name)
{
	int location = UNIFORM_INVALID_LOCATION;
	location = glGetUniformLocation(m_ShaderProgram, a_Name.GetName().c_str());
	Utility::Internal::CheckOGLError();
	return location;
}

unsigned int Lux::Core::Internal::ShaderOGL::GetAttribLocation(const Key& a_Name)
{
	unsigned int location = glGetAttribLocation(m_ShaderProgram, a_Name.GetName().c_str());
	Utility::Internal::CheckOGLError();
	return location;
}

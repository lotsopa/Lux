#include "LuxPCH.h"
#include "LuxShader.h"
#include "LuxShaderOGL.h"

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

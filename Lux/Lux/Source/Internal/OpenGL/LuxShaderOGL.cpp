#include "LuxPCH.h"
#include "LuxKey.h"
#include "LuxBufferOGL.h"
#include "LuxShader.h"
#include "LuxShaderOGL.h"
#include "LuxErrorCheckOGL.h"



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

unsigned int Lux::Core::Internal::ShaderOGL::GetAttribLocation(const Key& a_Name)
{
	unsigned int location = glGetAttribLocation(m_ShaderProgram, a_Name.GetName().c_str());
	Utility::Internal::CheckOGLError();
	return location;
}

void Lux::Core::Internal::ShaderOGL::Update()
{
	for (unsigned int i = 0; i < m_UniformBuffers.size(); ++i)
	{
		m_UniformBuffers[i].Update();
	}
}

void Lux::Core::Internal::ShaderOGL::InitializeUniformBuffer(const Key& a_Name, ShaderUniformBuffer& a_Buffer, ShaderProgram a_Type)
{
	for (unsigned int i = 0; i < m_InitializedConstantBuffers.size(); ++i)
	{
		if (m_InitializedConstantBuffers[i] == a_Name)
			return;
	}

	static unsigned int globalBindingPoint = 0;
	unsigned int blockIndex = glGetUniformBlockIndex(m_ShaderProgram, a_Name.GetName().c_str());
	glUniformBlockBinding(m_ShaderProgram, blockIndex, globalBindingPoint);
	Utility::Internal::CheckOGLError();
	m_UniformBuffers.emplace(m_UniformBuffers.end(), a_Buffer, globalBindingPoint);
	Utility::Internal::CheckOGLError();
	globalBindingPoint++;
	m_InitializedConstantBuffers.push_back(a_Name);
}

unsigned int Lux::Core::Internal::ShaderOGL::GetUniformLocation(const Key& a_Name)
{
	unsigned int loc = UINT_MAX;

	loc = glGetUniformLocation(m_ShaderProgram, a_Name.GetName().c_str());

	if (loc == UINT_MAX)
	{
		Utility::ThrowError("Failed to retrieve uniform location from shader.");
	}
	return loc;
}

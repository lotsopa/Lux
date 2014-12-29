#include "LuxPCH.h"
#include "LuxKey.h"
#include "LuxShader.h"
#include "LuxShaderOGL.h"
#include "LuxErrorCheckOGL.h"

#define FUNC_MAP_INSERT(a, b) m_FunctionMap.insert(std::make_pair(a, std::bind(&b, this, std::placeholders::_1, std::placeholders::_2)))
#define UNIFORM_INVALID_LOCATION -1

Lux::Core::Internal::ShaderOGL::ShaderOGL(std::vector<unsigned int>& a_LoadedShaders)
{
	m_ShaderProgram = CreateShaderProgram(a_LoadedShaders);
	LuxAssert(m_ShaderProgram != 0);
	FUNC_MAP_INSERT(Lux::Core::ShaderVariableType::VALUE_INT, Lux::Core::Internal::ShaderOGL::SetUniformInt);
	FUNC_MAP_INSERT(Lux::Core::ShaderVariableType::VALUE_FLOAT, Lux::Core::Internal::ShaderOGL::SetUniformFloat);
	FUNC_MAP_INSERT(Lux::Core::ShaderVariableType::VALUE_VEC2, Lux::Core::Internal::ShaderOGL::SetUniformVec2);
	FUNC_MAP_INSERT(Lux::Core::ShaderVariableType::VALUE_VEC3, Lux::Core::Internal::ShaderOGL::SetUniformVec3);
	FUNC_MAP_INSERT(Lux::Core::ShaderVariableType::VALUE_VEC4, Lux::Core::Internal::ShaderOGL::SetUniformVec4);
	FUNC_MAP_INSERT(Lux::Core::ShaderVariableType::VALUE_MAT4X4, Lux::Core::Internal::ShaderOGL::SetUniformMat4x4);
	FUNC_MAP_INSERT(Lux::Core::ShaderVariableType::VALUE_MAT3X3, Lux::Core::Internal::ShaderOGL::SetUniformMat3x3);
}

Lux::Core::Internal::ShaderOGL::~ShaderOGL()
{
	m_FunctionMap.clear();
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

void Lux::Core::Internal::ShaderOGL::SetUniformFloat(const Key& a_Name, void* a_Val)
{
	unsigned int location = GetUniformLocation(a_Name);
	float value = *(float*)a_Val;
	glUniform1f(location, value);
}

void Lux::Core::Internal::ShaderOGL::SetUniformInt(const Key& a_Name, void* a_Val)
{
	unsigned int location = GetUniformLocation(a_Name);
	int value = *(int*)a_Val;
	glUniform1i(location, value);
}

void Lux::Core::Internal::ShaderOGL::SetUniformVec2(const Key& a_Name, void* a_Val)
{
	unsigned int location = GetUniformLocation(a_Name);
	glUniform2fv(location, 1, (float*)a_Val);
}

void Lux::Core::Internal::ShaderOGL::SetUniformVec3(const Key& a_Name, void* a_Val)
{
	unsigned int location = GetUniformLocation(a_Name);
	glUniform3fv(location, 1,(float*)a_Val);
}

void Lux::Core::Internal::ShaderOGL::SetUniformVec4(const Key& a_Name, void* a_Val)
{
	unsigned int location = GetUniformLocation(a_Name);
	glUniform4fv(location, 1, (float*)a_Val);
}

void Lux::Core::Internal::ShaderOGL::SetUniformMat3x3(const Key& a_Name, void* a_Val)
{
	unsigned int location = GetUniformLocation(a_Name);
	glUniformMatrix3fv(location, 1, GL_FALSE, (float*)a_Val);
}

void Lux::Core::Internal::ShaderOGL::SetUniformMat4x4(const Key& a_Name, void* a_Val)
{
	unsigned int location = GetUniformLocation(a_Name);
	glUniformMatrix4fv(location, 1, GL_FALSE, (float*)a_Val);
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

void Lux::Core::Internal::ShaderOGL::SetUniformVariable(const Key& a_Name, ShaderVariable& a_Var)
{
	m_Uniforms.insert(std::make_pair(a_Name, a_Var));
}

void Lux::Core::Internal::ShaderOGL::Update()
{
	VariableMap::iterator it;

	for (it = m_Uniforms.begin(); it != m_Uniforms.end(); ++it)
	{
		ShaderVariableType type = it->second.GetType();
		m_FunctionMap[type](it->first, it->second.GetData());
	}
}

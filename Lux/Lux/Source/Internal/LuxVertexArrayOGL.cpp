#include "LuxPCH.h"
#include "LuxErrorCheckOGL.h"
#include "LuxVertexArrayOGL.h"

Lux::Core::Internal::VertexArrayOGL::VertexArrayOGL()
{
	glGenVertexArrays(1, &m_VAO);
	Utility::Internal::CheckOGLError();
}

Lux::Core::Internal::VertexArrayOGL::~VertexArrayOGL()
{
	glDeleteVertexArrays(1, &m_VAO);
	Utility::Internal::CheckOGLError();
}

void Lux::Core::Internal::VertexArrayOGL::Bind()
{
	glBindVertexArray(m_VAO);
	Utility::Internal::CheckOGLError();
}

void Lux::Core::Internal::VertexArrayOGL::Unbind()
{
	glBindVertexArray(0);
	Utility::Internal::CheckOGLError();
}

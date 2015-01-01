#include "LuxPCH.h"
#include "LuxBufferOGL.h"
#include "LuxErrorCheckOGL.h"

Lux::Core::Internal::BufferOGL::BufferOGL(GLenum a_BuffType, GLenum a_DrawMode) :
m_BufferType(a_BuffType), m_DrawMode(a_DrawMode), m_Buffer(-1)
{
	glGenBuffers(1, &m_Buffer);
	Utility::Internal::CheckOGLError();
}

Lux::Core::Internal::BufferOGL::~BufferOGL()
{
	glDeleteBuffers(1, &m_Buffer);
	Utility::Internal::CheckOGLError();
}

void Lux::Core::Internal::BufferOGL::Bind()
{
	glBindBuffer(m_BufferType, m_Buffer);
	Utility::Internal::CheckOGLError();
}

void Lux::Core::Internal::BufferOGL::Unbind()
{
	glBindBuffer(m_BufferType, 0);
	Utility::Internal::CheckOGLError();
}

void Lux::Core::Internal::BufferOGL::SetData(const void* a_Data, unsigned int a_DataSize)
{
	glBufferData(m_BufferType, a_DataSize, a_Data, m_DrawMode);
	Utility::Internal::CheckOGLError();
}

#include "LuxPCH.h"
#include "LuxBufferOGL.h"
#include "LuxVertexArrayOGL.h"
#include "LuxErrorCheckOGL.h"
#include "LuxVertex.h"
#include "LuxSubMesh.h"
#include "LuxSubMeshOGL.h"
#include "LuxKey.h"
#include "LuxShader.h"
#include "LuxShaderOGL.h"

Lux::Core::Internal::SubMeshOGL::SubMeshOGL() : SubMesh(),
m_VertexArray(),
m_IndexBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW),
m_VertexBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW)
{
	m_VertexArray.Bind();

	m_VertexBuffer.Bind();
	m_VertexBuffer.SetData(m_Vertices, sizeof(Vertex) * m_NumVertices);

	m_IndexBuffer.Bind();
	m_IndexBuffer.SetData(m_Indices, sizeof(unsigned int) * m_NumIndices);

	m_VertexArray.Unbind();
	SafeDeleteAttributes();
}

Lux::Core::Internal::SubMeshOGL::SubMeshOGL(const SubMesh& a_SubMesh) : SubMesh(a_SubMesh),
m_VertexArray(),
m_IndexBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW),
m_VertexBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW)
{
	m_VertexArray.Bind();

	m_VertexBuffer.Bind();
	m_VertexBuffer.SetData(m_Vertices, sizeof(Vertex) * m_NumVertices);

	m_IndexBuffer.Bind();
	m_IndexBuffer.SetData(m_Indices, sizeof(unsigned int) * m_NumIndices);

	m_VertexArray.Unbind();
	SafeDeleteAttributes();
}

Lux::Core::Internal::SubMeshOGL::SubMeshOGL(aiMesh& a_Mesh) : SubMesh(a_Mesh),
m_VertexArray(),
m_VertexBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW),
m_IndexBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW)
{
	m_VertexArray.Bind();

	m_VertexBuffer.Bind();
	m_VertexBuffer.SetData(m_Vertices, sizeof(Vertex) * m_NumVertices);

	m_IndexBuffer.Bind();
	m_IndexBuffer.SetData(m_Indices, sizeof(unsigned int) * m_NumIndices);

	m_VertexArray.Unbind();
	SafeDeleteAttributes();
}

Lux::Core::Internal::SubMeshOGL::~SubMeshOGL()
{
	
}

void Lux::Core::Internal::SubMeshOGL::PreRender()
{
	m_VertexArray.Bind();
}

void Lux::Core::Internal::SubMeshOGL::PostRender()
{
	m_VertexArray.Unbind();
}

void Lux::Core::Internal::SubMeshOGL::ConnectWithShader(ShaderOGL* a_Shader)
{
	m_VertexArray.Bind();
	unsigned int posLoc = a_Shader->GetAttribLocation("in_Position");
	glEnableVertexAttribArray(posLoc);
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), MEMBER_OFFSET(Vertex, m_Position));
	Utility::Internal::CheckOGLError();

	unsigned int normalLoc = a_Shader->GetAttribLocation("in_Normal");
	glEnableVertexAttribArray(normalLoc);
	glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), MEMBER_OFFSET(Vertex, m_Normal));
	Utility::Internal::CheckOGLError();

	m_VertexArray.Unbind();
}

#include "LuxPCH.h"
#include "LuxBufferOGL.h"
#include "LuxVertexArrayOGL.h"
#include "LuxErrorCheckOGL.h"
#include "LuxVertex.h"
#include "LuxSubMesh.h"
#include "LuxSubMeshOGL.h"

Lux::Core::Internal::SubMeshOGL::SubMeshOGL() : SubMesh(),
m_VertexArray(),
m_IndexBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW),
m_VertexBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW)
{
	m_VertexArray.Bind();
	m_VertexBuffer.Bind();
	m_IndexBuffer.Bind();

	m_VertexBuffer.SetData(m_Vertices, sizeof(Vertex) * m_NumVertices);
	m_IndexBuffer.SetData(m_Indices, sizeof(unsigned int) * m_NumIndices);

	m_VertexBuffer.Unbind();
	m_IndexBuffer.Unbind();
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
	m_IndexBuffer.Bind();

	m_VertexBuffer.SetData(m_Vertices, sizeof(Vertex) * m_NumVertices);
	m_IndexBuffer.SetData(m_Indices, sizeof(unsigned int) * m_NumIndices);

	m_VertexBuffer.Unbind();
	m_IndexBuffer.Unbind();
	m_VertexArray.Unbind();

	SafeDeleteAttributes();
}

Lux::Core::Internal::SubMeshOGL::SubMeshOGL(aiMesh& a_Mesh) : SubMesh(a_Mesh),
m_VertexArray(),
m_IndexBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW),
m_VertexBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW)
{
	m_VertexArray.Bind();
	m_VertexBuffer.Bind();
	m_IndexBuffer.Bind();

	m_VertexBuffer.SetData(m_Vertices, sizeof(Vertex) * m_NumVertices);
	m_IndexBuffer.SetData(m_Indices, sizeof(unsigned int) * m_NumIndices);

	m_VertexBuffer.Unbind();
	m_IndexBuffer.Unbind();
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

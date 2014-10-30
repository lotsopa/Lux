#include "LuxPCH.h"
#include "LuxVertex.h"

Lux::Core::Vertex::Vertex()
{

}

Lux::Core::Vertex::Vertex(vec3 a_Pos, vec3 a_Normal, vec2 a_TexCoords, vec3 a_Tangent, vec3 a_Bitangent) :
m_Position(a_Pos),
m_Normal(a_Normal),
m_TexCoords(a_TexCoords),
m_Tangent(a_Tangent),
m_Bitangent(a_Bitangent)
{

}

Lux::Core::Vertex::~Vertex()
{

}

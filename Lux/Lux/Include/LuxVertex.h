#ifndef LUX_VERTEX_H
#define LUX_VERTEX_H

namespace Lux
{
	namespace Core
	{
		class Vertex
		{
		public:
			Vertex();
			Vertex(vec3 a_Pos, vec3 a_Normal, vec2 a_TexCoords, vec3 a_Tangent, vec3 a_Bitangent);
			~Vertex();

			vec3 m_Position;
			vec3 m_Normal;
			vec2 m_TexCoords;
			vec3 m_Tangent;
			vec3 m_Bitangent;
			vec4 m_Color;

		};
	}
}

#endif
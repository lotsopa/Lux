#ifndef LUX_VERTEX_ARRAY_OGL_H
#define LUX_VERTEX_ARRAY_OGL_H

namespace Lux
{
	namespace Core
	{
		namespace Internal
		{
			class VertexArrayOGL
			{
			public:
				VertexArrayOGL();
				~VertexArrayOGL();

				void Bind();
				void Unbind();

			private:
				unsigned int m_VAO;
			};
		}
	}
}

#endif
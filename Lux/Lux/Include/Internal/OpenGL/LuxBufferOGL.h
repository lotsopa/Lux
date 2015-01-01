#ifndef LUX_BUFFER_OGL_H
#define LUX_BUFFER_OGL_H

namespace Lux
{
	namespace Core
	{
		namespace Internal
		{
			class BufferOGL
			{
			public:
				BufferOGL(GLenum a_BuffType, GLenum a_DrawMode);
				~BufferOGL();

				void Bind();
				void Unbind();
				void SetData(const void* a_Data, unsigned int a_DataSize);

			private:
				unsigned int m_Buffer;
				GLenum m_BufferType;
				GLenum m_DrawMode;
			};
		}
	}
}

#endif
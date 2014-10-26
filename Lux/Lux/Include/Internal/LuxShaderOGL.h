#ifndef LUX_SHADER_OGL_H
#define LUX_SHADER_OGL_H

namespace Lux
{
	namespace Core
	{
		namespace Internal
		{
			class ShaderOGL : public Shader
			{
			public:
				ShaderOGL();
				virtual ~ShaderOGL();

				inline const unsigned int GetShaderProgram() { return m_ShaderProgram;  }

			private:
				unsigned int m_ShaderProgram;
			};
		}
	}
}

#endif
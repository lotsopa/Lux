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
				ShaderOGL(std::vector<unsigned int>& a_LoadedShaders);
				virtual ~ShaderOGL();
				virtual void Activate();
				virtual void Deactivate();

				virtual void SetUniformFloat(const Key& a_Name, float a_Val);
				virtual void SetUniformInt(const Key& a_Name, int a_Val);
				virtual void SetUniformVec2(const Key& a_Name, const vec2& a_Vec);
				virtual void SetUniformVec3(const Key& a_Name, const vec3& a_Vec);
				virtual void SetUniformVec4(const Key& a_Name, const vec4 a_Vec);
				virtual void SetUniformMat3x3(const Key& a_Name, const mat3x3& a_Mat);
				virtual void SetUniformMat4x4(const Key& a_Name, const mat4x4& a_Mat);

				inline const unsigned int GetShaderProgram() { return m_ShaderProgram;  }
				unsigned int GetAttribLocation(const Key& a_Name);

			private:
				unsigned int GetUniformLocation(const Key& a_Name);
				unsigned int m_ShaderProgram;
				unsigned int CreateShaderProgram(std::vector<unsigned int>& shaders);
			};
		}
	}
}

#endif
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

				virtual void SetUniformVariable(const Key& a_Name, ShaderVariable& a_Var);

				inline const unsigned int GetShaderProgram() { return m_ShaderProgram;  }
				unsigned int GetAttribLocation(const Key& a_Name);
				virtual void Update();

			private:
				unsigned int GetUniformLocation(const Key& a_Name);
				unsigned int m_ShaderProgram;
				unsigned int CreateShaderProgram(std::vector<unsigned int>& shaders);
				typedef std::map<Key, ShaderVariable> VariableMap;
				
				VariableMap m_Uniforms;

				typedef std::unordered_map<ShaderVariableType, std::function<void(const Key&, void*)>> FunctionMap;
				virtual void SetUniformFloat(const Key& a_Name, void* a_Val);
				virtual void SetUniformInt(const Key& a_Name, void* a_Val);
				virtual void SetUniformVec2(const Key& a_Name, void* a_Val);
				virtual void SetUniformVec3(const Key& a_Name, void* a_Val);
				virtual void SetUniformVec4(const Key& a_Name, void* a_Val);
				virtual void SetUniformMat3x3(const Key& a_Name, void* a_Val);
				virtual void SetUniformMat4x4(const Key& a_Name, void* a_Val);

				FunctionMap m_FunctionMap;
			};
		}
	}
}

#endif
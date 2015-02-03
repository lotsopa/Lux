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

				virtual void InitializeUniformBuffer(const Key& a_Name, ShaderUniformBuffer& a_Buffer, ShaderProgram a_Type);
				inline const unsigned int GetShaderProgram() { return m_ShaderProgram;  }
				unsigned int GetAttribLocation(const Key& a_Name);
				virtual void Update();
				unsigned int GetUniformLocation(const Key& a_Name);
			private:
				unsigned int m_ShaderProgram;
				unsigned int CreateShaderProgram(std::vector<unsigned int>& shaders);

				struct UniformBufferEntry
				{
					UniformBufferEntry(ShaderUniformBuffer& a_Buffer, unsigned int a_BlockIndex) :
						m_Buffer(a_Buffer), m_BlockIndex(a_BlockIndex)
					{
						m_GLBuffer = new BufferOGL(GL_UNIFORM_BUFFER, GL_STATIC_DRAW);
						m_GLBuffer->Bind();
						unsigned int bufferSize = a_Buffer.GetSizeInBytes();
						m_GLBuffer->SetData(nullptr, bufferSize);
						m_GLBuffer->BindBufferBase(m_BlockIndex);
						m_GLBuffer->Unbind();
					}

					UniformBufferEntry& operator=(const UniformBufferEntry& other) // copy assignment
					{
						this->m_BlockIndex = other.m_BlockIndex;
						this->m_Buffer = other.m_Buffer;
						this->m_GLBuffer = other.m_GLBuffer;
						return *this;
					}

					~UniformBufferEntry()
					{
						Utility::SafePtrDelete(m_GLBuffer);
					}

					void Update()
					{
						std::queue<unsigned int>& modifiedValues = m_Buffer.GetModifiedValues();
						
						while (!modifiedValues.empty())
						{
							unsigned int valIdx = modifiedValues.front();
							modifiedValues.pop();
							ShaderVariable var = m_Buffer.GetVariable(valIdx);
							m_GLBuffer->Bind();
							m_GLBuffer->SetSubData(var.GetData(), var.GetDataSizeBytes(), m_Buffer.GetVariableOffsetinBytes(valIdx));
							m_GLBuffer->Unbind();
						}
					}

					BufferOGL* m_GLBuffer;
					ShaderUniformBuffer& m_Buffer;
					unsigned int m_BlockIndex;
				};
				std::vector<UniformBufferEntry> m_UniformBuffers;
				std::vector<Key> m_InitializedConstantBuffers;
			};
		}
	}
}

#endif
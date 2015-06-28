#ifndef LUX_SHADER_H
#define LUX_SHADER_H

#define LUX_SHADER_PACK_OFFSET 16
namespace Lux
{
	namespace Core
	{
		enum ShaderVariableType
		{
			VALUE_INT = 0,
			VALUE_FLOAT,
			VALUE_VEC2,
			VALUE_VEC3,
			VALUE_VEC4,
			VALUE_MAT4X4,
			VALUE_MAT3X3
		};

		class ShaderVariable
		{
		public:
			ShaderVariable() : m_Data(nullptr), m_DataSizeBytes(0)
			{

			}

			ShaderVariable(ShaderVariableType a_Type, const void* a_VarPtr, const unsigned int a_DataSizeBytes) : 
				m_Data(a_VarPtr), m_Type(a_Type), m_DataSizeBytes(a_DataSizeBytes)
			{

			}

			void Set(const void* a_Data, const unsigned int a_DataSizeBytes, ShaderVariableType a_Type) { m_Data = a_Data; m_Type = a_Type; m_DataSizeBytes = a_DataSizeBytes; }
			ShaderVariableType GetType() { return m_Type; }
			const void* GetData() { return m_Data; }
			const unsigned int GetDataSizeBytes() { return m_DataSizeBytes; }
		private:
			const void* m_Data;
			unsigned int m_DataSizeBytes;
			ShaderVariableType m_Type;
		};

		class ShaderUniformBuffer
		{
		public:
			ShaderUniformBuffer(unsigned int a_NumVars) : m_NumVariables(a_NumVars), m_RawData(nullptr), 
				m_AlignedByteSize(0), m_RawDataSize(0)
			{
				m_Variables = new ShaderVariable[m_NumVariables];
			}

			~ShaderUniformBuffer()
			{
				Utility::SafePtrDelete(m_Variables);
				
				Utility::SafeArrayDelete(m_RawData);
			}

			void SetVariable(const unsigned int a_Idx, ShaderVariable& a_Var)
			{
				LuxAssert(a_Idx < m_NumVariables);
				m_Variables[a_Idx].Set(a_Var.GetData(), a_Var.GetDataSizeBytes(), a_Var.GetType());
				m_ModifiedValues.push(a_Idx);
			}

			const ShaderVariable& GetVariable(const unsigned int a_Idx) const
			{
				LuxAssert(a_Idx < m_NumVariables);
				return m_Variables[a_Idx];
			}

			const unsigned int GetSizeInBytes()
			{
				unsigned int size = 0;
				for (unsigned int i = 0; i < m_NumVariables; i++)
				{
					size += m_Variables[i].GetDataSizeBytes();
				}
				return size;
			}

			const unsigned int GetVariableOffsetinBytes(const unsigned int a_Idx)
			{
				LuxAssert(a_Idx < m_NumVariables);
				unsigned int size = 0;
				for (unsigned int i = 0; i < a_Idx; i++)
				{
					size += m_Variables[i].GetDataSizeBytes();
				}
				return size;
			}

			// Not checked
			void* GetRawValueArray()
			{	
				unsigned int currindex = 0;
				unsigned int currByteBoundary = 0;
				for (unsigned int i = 0; i < m_NumVariables; i++)
				{
					LuxAssert(currindex < m_RawDataSize);
					ShaderVariableType type = m_Variables[i].GetType();
					switch (type)
					{
					case VALUE_FLOAT:
					{
						currByteBoundary += sizeof(float);
						memcpy(&m_RawData[currindex], m_Variables[i].GetData(), sizeof(float));
						currindex++;
					}
						break;

					case VALUE_INT:
					{
						currByteBoundary += sizeof(int);
						memcpy(&m_RawData[currindex], m_Variables[i].GetData(), sizeof(int));
						currindex++;
					}
						break;

					case VALUE_VEC2:
					{
						if (currByteBoundary >= LUX_SHADER_PACK_OFFSET - sizeof(float))
						{
							// Skip an index and go for the next one
							currindex++;
							currByteBoundary += sizeof(vec2);
							memcpy(&m_RawData[currindex], m_Variables[i].GetData(), sizeof(vec2));
							currindex += 2;

						}
						else
						{
							currByteBoundary += sizeof(vec2);
							memcpy(&m_RawData[currindex], m_Variables[i].GetData(), sizeof(vec2));
							currindex += 2;
						}
					}
						break;

					case VALUE_VEC3:
					{
						if (currByteBoundary >= LUX_SHADER_PACK_OFFSET - sizeof(float))
						{
							// Skip an index and go for the next one
							currindex++;
							currByteBoundary += sizeof(vec3);
							memcpy(&m_RawData[currindex], m_Variables[i].GetData(), sizeof(vec3));
							currindex += 3;

						}
						else if (currByteBoundary >= LUX_SHADER_PACK_OFFSET - sizeof(vec2))
						{
							// Skip an index and go for the next one
							currindex += 2;
							currByteBoundary += sizeof(vec3);
							memcpy(&m_RawData[currindex], m_Variables[i].GetData(), sizeof(vec3));
							currindex += 3;
						}
						else
						{
							currByteBoundary += sizeof(vec3);
							memcpy(&m_RawData[currindex], m_Variables[i].GetData(), sizeof(vec3));
							currindex += 3;
						}
					}
						break;

					case VALUE_VEC4:
					{
						if (currByteBoundary >= LUX_SHADER_PACK_OFFSET - sizeof(float))
						{
							// Skip an index and go for the next one
							currindex++;
							currByteBoundary += sizeof(vec4);
							memcpy(&m_RawData[currindex], m_Variables[i].GetData(), sizeof(vec4));
							currindex += 4;
						}
						else if (currByteBoundary >= LUX_SHADER_PACK_OFFSET - sizeof(vec2))
						{
							// Skip an index and go for the next one
							currindex += 2;
							currByteBoundary += sizeof(vec4);
							memcpy(&m_RawData[currindex], m_Variables[i].GetData(), sizeof(vec4));
							currindex += 4;
						}
						else if (currByteBoundary >= LUX_SHADER_PACK_OFFSET - sizeof(vec3))
						{
							// Skip an index and go for the next one
							currindex += 3;
							currByteBoundary += sizeof(vec4);
							memcpy(&m_RawData[currindex], m_Variables[i].GetData(), sizeof(vec4));
							currindex += 4;
						}
						else
						{
							currByteBoundary += sizeof(vec4);
							memcpy(&m_RawData[currindex], m_Variables[i].GetData(), sizeof(vec4));
							currindex += 4;
						}
					}
						break;

					case VALUE_MAT3X3:
					{
						// Not tested
						if (currByteBoundary >= LUX_SHADER_PACK_OFFSET - sizeof(float))
						{
							currindex++;
							currByteBoundary += sizeof(vec4) * 3;
							memcpy(&m_RawData[currindex], m_Variables[i].GetData(), sizeof(vec3));
							currindex += 4;

							// Next 3 components
							float* castedPtr = (float*)m_Variables[i].GetData();
							memcpy(&m_RawData[currindex], &castedPtr[3], sizeof(vec3));
							currindex += 4;

							// Final 3
							memcpy(&m_RawData[currindex], &castedPtr[6], sizeof(vec3));
							currindex += 4;
						}
						else if (currByteBoundary >= LUX_SHADER_PACK_OFFSET - sizeof(vec2))
						{
							// Skip an index and go for the next one
							currindex += 2;
							currByteBoundary += sizeof(vec4) * 3;
							memcpy(&m_RawData[currindex], m_Variables[i].GetData(), sizeof(vec3));
							currindex += 4;

							// Next 3 components
							float* castedPtr = (float*)m_Variables[i].GetData();
							memcpy(&m_RawData[currindex], &castedPtr[3], sizeof(vec3));
							currindex += 4;

							// Final 3
							memcpy(&m_RawData[currindex], &castedPtr[6], sizeof(vec3));
							currindex += 4;
						}
						else
						{
							currByteBoundary += sizeof(vec4) * 3;
							memcpy(&m_RawData[currindex], m_Variables[i].GetData(), sizeof(vec3));
							currindex += 4;

							// Next 3 components
							float* castedPtr = (float*)m_Variables[i].GetData();
							memcpy(&m_RawData[currindex], &castedPtr[3], sizeof(vec3));
							currindex += 4;

							// Final 3
							memcpy(&m_RawData[currindex], &castedPtr[6], sizeof(vec3));
							currindex += 4;
						}
					}
						break;

					case VALUE_MAT4X4:
					{				
						if (currByteBoundary >= LUX_SHADER_PACK_OFFSET - sizeof(float))
						{
							// Skip an index and go for the next one
							currindex++;
							currByteBoundary += sizeof(mat4);
							memcpy(&m_RawData[currindex], m_Variables[i].GetData(), sizeof(mat4));
							currindex += 16;

						}
						else if (currByteBoundary >= LUX_SHADER_PACK_OFFSET - sizeof(vec2))
						{
							// Skip an index and go for the next one
							currindex += 2;
							currByteBoundary += sizeof(mat4);
							memcpy(&m_RawData[currindex], m_Variables[i].GetData(), sizeof(mat4));
							currindex += 16;
						}
						else if (currByteBoundary >= LUX_SHADER_PACK_OFFSET - sizeof(vec3))
						{
							// Skip an index and go for the next one
							currindex += 3;
							currByteBoundary += sizeof(mat4);
							memcpy(&m_RawData[currindex], m_Variables[i].GetData(), sizeof(mat4));
							currindex += 16;
						}
						else
						{
							currByteBoundary += sizeof(mat4);
							memcpy(&m_RawData[currindex], m_Variables[i].GetData(), sizeof(mat4));
							currindex += 16;
						}
					}
						break;

					}

					if (currByteBoundary == LUX_SHADER_PACK_OFFSET)
					{
						currByteBoundary = 0;
					}
					else if (currByteBoundary > LUX_SHADER_PACK_OFFSET)
					{
						if (currByteBoundary % LUX_SHADER_PACK_OFFSET != 0)
						{
							currByteBoundary = currByteBoundary - LUX_SHADER_PACK_OFFSET;
						}
						else
						{
							currByteBoundary = 0;
						}
						
					}
					else if (currByteBoundary > LUX_SHADER_PACK_OFFSET * 2)
					{
						if (currByteBoundary % LUX_SHADER_PACK_OFFSET != 0)
						{
							currByteBoundary = currByteBoundary - LUX_SHADER_PACK_OFFSET * 2;
						}
						else
						{
							currByteBoundary = 0;
						}
					}
				}

				return m_RawData;
			}

			void AllocateRawData(unsigned int a_AlignedSize)
			{
				m_AlignedByteSize = a_AlignedSize;
				m_RawDataSize = m_AlignedByteSize / sizeof(float);
				m_RawData = new float[m_RawDataSize];
			}

			std::queue<unsigned int>& GetModifiedValues() { return m_ModifiedValues; }
		private:
			unsigned int m_NumVariables;
			ShaderVariable* m_Variables;
			std::queue<unsigned int> m_ModifiedValues;
			float* m_RawData;
			unsigned int m_AlignedByteSize;
			unsigned int m_RawDataSize;
		};

		class Shader
		{
		public:
			Shader();
			virtual ~Shader();

			virtual void Activate() = 0;
			virtual void Deactivate() = 0;

			virtual void InitializeUniformBuffer(const Key& a_Name, ShaderUniformBuffer& a_Buffer, ShaderProgram a_Type) = 0;
			virtual void Update() = 0;

		};
	}
}

#endif
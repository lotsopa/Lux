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

			ShaderVariable(ShaderVariableType a_Type, void* a_VarPtr, const unsigned int a_DataSizeBytes) : 
				m_Data(a_VarPtr), m_Type(a_Type), m_DataSizeBytes(a_DataSizeBytes)
			{

			}

			void Set(void* a_Data, const unsigned int a_DataSizeBytes, ShaderVariableType a_Type) { m_Data = a_Data; m_Type = a_Type; m_DataSizeBytes = a_DataSizeBytes; }
			ShaderVariableType GetType() { return m_Type; }
			void* GetData() { return m_Data; }
			const unsigned int GetDataSizeBytes() { return m_DataSizeBytes; }
		private:
			void* m_Data;
			unsigned int m_DataSizeBytes;
			ShaderVariableType m_Type;
		};

		class ShaderUniformBuffer
		{
		public:
			ShaderUniformBuffer(unsigned int a_NumVars) : m_NumVariables(a_NumVars), m_RawData(nullptr), m_AlignedSize(0)
			{
				m_Variables = new ShaderVariable[m_NumVariables];
			}

			~ShaderUniformBuffer()
			{
				Utility::SafePtrDelete(m_Variables);
				
				if (m_RawData)
				{
					LUX_ALIGNED_FREE(m_RawData);
				}
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
					LuxAssert(currindex < m_AlignedSize);
					ShaderVariableType type = m_Variables->GetType();
					switch (type)
					{
					case VALUE_FLOAT:
					{
						currByteBoundary += sizeof(float);
						memcpy(&m_RawData[currindex], m_Variables->GetData(), sizeof(float));
						currindex++;
					}
						break;

					case VALUE_INT:
					{
						currByteBoundary += sizeof(int);
						memcpy(&m_RawData[currindex], m_Variables->GetData(), sizeof(int));
						currindex++;
					}
						break;

					case VALUE_VEC2:
					{
						if (currByteBoundary >= LUX_SHADER_PACK_OFFSET - sizeof(float))
						{
							// Skip an index and go for the next one
							currindex++;
							currByteBoundary += sizeof(float) * 2;
							memcpy(&m_RawData[currindex], m_Variables->GetData(), sizeof(float) * 2);
							currindex += 2;

						}
						else
						{
							currByteBoundary += sizeof(float) * 2;
							memcpy(&m_RawData[currindex], m_Variables->GetData(), sizeof(float) * 2);
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
							currByteBoundary += sizeof(float) * 3;
							memcpy(&m_RawData[currindex], m_Variables->GetData(), sizeof(float) * 3);
							currindex += 3;

						}
						else if (currByteBoundary >= LUX_SHADER_PACK_OFFSET - sizeof(float) * 2)
						{
							// Skip an index and go for the next one
							currindex += 2;
							currByteBoundary += sizeof(float) * 3;
							memcpy(&m_RawData[currindex], m_Variables->GetData(), sizeof(float) * 3);
							currindex += 3;
						}
						else
						{
							currByteBoundary += sizeof(float) * 3;
							memcpy(&m_RawData[currindex], m_Variables->GetData(), sizeof(float) * 3);
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
							currByteBoundary += sizeof(float) * 4;
							memcpy(&m_RawData[currindex], m_Variables->GetData(), sizeof(float) * 4);
							currindex += 4;

						}
						else if (currByteBoundary >= LUX_SHADER_PACK_OFFSET - sizeof(float) * 2)
						{
							// Skip an index and go for the next one
							currindex += 2;
							currByteBoundary += sizeof(float) * 4;
							memcpy(&m_RawData[currindex], m_Variables->GetData(), sizeof(float) * 4);
							currindex += 4;
						}
						else if (currByteBoundary >= LUX_SHADER_PACK_OFFSET - sizeof(float) * 3)
						{
							// Skip an index and go for the next one
							currindex += 3;
							currByteBoundary += sizeof(float) * 4;
							memcpy(&m_RawData[currindex], m_Variables->GetData(), sizeof(float) * 4);
							currindex += 4;
						}
						else
						{
							currByteBoundary += sizeof(float) * 4;
							memcpy(&m_RawData[currindex], m_Variables->GetData(), sizeof(float) * 4);
							currindex += 4;
						}
					}
						break;

					case VALUE_MAT3X3:
					{
						currByteBoundary += sizeof(float) * 9;
						memcpy(&m_RawData[currindex], m_Variables->GetData(), sizeof(float) * 9);
						currindex += 9;
					}
						break;

					case VALUE_MAT4X4:
					{
						if (currByteBoundary >= LUX_SHADER_PACK_OFFSET - sizeof(float))
						{
							// Skip an index and go for the next one
							currindex++;
							currByteBoundary += sizeof(float) * 16;
							memcpy(&m_RawData[currindex], m_Variables->GetData(), sizeof(float) * 16);
							currindex += 16;

						}
						else if (currByteBoundary >= LUX_SHADER_PACK_OFFSET - sizeof(float) * 2)
						{
							// Skip an index and go for the next one
							currindex += 2;
							currByteBoundary += sizeof(float) * 16;
							memcpy(&m_RawData[currindex], m_Variables->GetData(), sizeof(float) * 16);
							currindex += 16;
						}
						else if (currByteBoundary >= LUX_SHADER_PACK_OFFSET - sizeof(float) * 3)
						{
							// Skip an index and go for the next one
							currindex += 3;
							currByteBoundary += sizeof(float) * 16;
							memcpy(&m_RawData[currindex], m_Variables->GetData(), sizeof(float) * 16);
							currindex += 16;
						}
						else
						{
							currByteBoundary += sizeof(float) * 16;
							memcpy(&m_RawData[currindex], m_Variables->GetData(), sizeof(float) * 16);
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
						currByteBoundary = currByteBoundary - LUX_SHADER_PACK_OFFSET;
					}
					else if (currByteBoundary > LUX_SHADER_PACK_OFFSET * 2)
					{
						currByteBoundary = currByteBoundary - LUX_SHADER_PACK_OFFSET * 2;
					}
				}

				return m_RawData;
			}

			void AllocateRawData(unsigned int a_AlignedSize)
			{
				m_AlignedSize = a_AlignedSize;
				m_RawData = (float*)LUX_ALIGNED_MALLOC(m_AlignedSize, LUX_SHADER_PACK_OFFSET);
			}

			std::queue<unsigned int>& GetModifiedValues() { return m_ModifiedValues; }
		private:
			unsigned int m_NumVariables;
			ShaderVariable* m_Variables;
			std::queue<unsigned int> m_ModifiedValues;
			float* m_RawData;
			unsigned int m_AlignedSize;
		};

		class Shader
		{
		public:
			Shader();
			virtual ~Shader();

			virtual void Activate() = 0;
			virtual void Deactivate() = 0;

			virtual void BindUniformBuffer(const Key& a_Name, ShaderUniformBuffer& a_Buffer, ShaderProgram a_Type) = 0;
			virtual void Update() = 0;

		};
	}
}

#endif
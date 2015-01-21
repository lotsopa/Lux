#ifndef LUX_SHADER_H
#define LUX_SHADER_H

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
			ShaderUniformBuffer(unsigned int a_NumVars) : m_NumVariables(a_NumVars)
			{
				m_Variables = new ShaderVariable[m_NumVariables];
			}

			~ShaderUniformBuffer()
			{
				Utility::SafePtrDelete(m_Variables);
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
					size += m_Variables->GetDataSizeBytes();
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

			std::queue<unsigned int>& GetModifiedValues() { return m_ModifiedValues; }
		private:
			unsigned int m_NumVariables;
			ShaderVariable* m_Variables;
			std::queue<unsigned int> m_ModifiedValues;
		};

		class Shader
		{
		public:
			Shader();
			virtual ~Shader();

			virtual void Activate() = 0;
			virtual void Deactivate() = 0;

			virtual void BindUniformBuffer(const Key& a_Name, ShaderUniformBuffer& a_Buffer) = 0;
			virtual void Update() = 0;

		};
	}
}

#endif
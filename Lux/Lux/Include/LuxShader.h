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
			ShaderVariable(ShaderVariableType a_Type, void* a_VarPtr) : 
				m_Data(a_VarPtr), m_Type(a_Type)
			{

			}

			ShaderVariableType GetType() { return m_Type; }
			void* GetData() { return m_Data; }
		private:
			void* m_Data;
			ShaderVariableType m_Type;
		};

		class Shader
		{
		public:
			Shader();
			virtual ~Shader();

			virtual void Activate() = 0;
			virtual void Deactivate() = 0;

			virtual void SetUniformVariable(const Key& a_Name, ShaderVariable& a_Var) = 0;
			virtual void Update() = 0;

		};
	}
}

#endif
#ifndef LUX_SHADER_H
#define LUX_SHADER_H

namespace Lux
{
	namespace Core
	{
		class Shader
		{
		public:
			Shader();
			virtual ~Shader();

			virtual void Activate() = 0;
			virtual void Deactivate() = 0;

			virtual void SetUniformFloat(const Key& a_Name, float a_Val) = 0;
			virtual void SetUniformInt(const Key& a_Name, int a_Val) = 0;
			virtual void SetUniformVec2(const Key& a_Name, const vec2& a_Vec) = 0;
			virtual void SetUniformVec3(const Key& a_Name, const vec3& a_Vec) = 0;
			virtual void SetUniformVec4(const Key& a_Name, const vec4 a_Vec) = 0;
			virtual void SetUniformMat3x3(const Key& a_Name, const mat3x3& a_Mat) = 0;
			virtual void SetUniformMat4x4(const Key& a_Name, const mat4x4& a_Mat) = 0;
		};
	}
}

#endif
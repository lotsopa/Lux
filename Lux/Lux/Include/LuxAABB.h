#ifndef LUX_AABB_H
#define LUX_AABB_H
/*
Simple Axis Aligned Bounding Box Class.
*/

namespace Lux
{
	namespace Core
	{
		class AABB
		{
		public:
			AABB();
			AABB(const vec3& a_Min, const vec3& a_Max);
			~AABB();

			inline const vec3& GetMax() const { return m_Max; }
			inline const vec3& GetMin() const { return m_Min; }
			inline const vec3& GetHalfExtents() { return m_HalfExtents; }

			void SetMax(const vec3& a_Max);
			void SetMin(const vec3& a_Min);


		private:
			vec3 m_Max;
			vec3 m_Min;
			vec3 m_HalfExtents;
		};
	}
}

#endif
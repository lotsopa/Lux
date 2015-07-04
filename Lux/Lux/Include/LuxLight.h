#ifndef LUX_LIGHT_H
#define LUX_LIGHT_H

namespace Lux
{
	namespace Graphics
	{
		class Light : public Core::Component
		{
		public:
			Light();
			virtual ~Light();

			enum LightType
			{
				DIRECTIONAL_LIGHT = 0,
				POINT_LIGHT
			};

			inline void SetColor(const vec4& a_Col) { m_Color = a_Col; }
			inline vec4& GetColor() { return m_Color; }

			inline void SetType(LightType a_Type) { m_Type = a_Type; }
			inline const LightType GetType() { return m_Type; }

			inline void SetDirection(const vec3& a_Dir) { m_Direction = a_Dir; }
			inline const vec3 GetDirection() { return m_Direction; }

			inline void Setintensity(float a_Int) { m_Intensity = a_Int; }
			inline const float GetIntensity() { return m_Intensity; }

		private:
			LightType m_Type;
			vec4 m_Color;
			vec3 m_Direction;
			float m_Intensity;

		protected:
			virtual void Reset();
			friend class Core::ComponentFactory;
		};
	}
}

#endif
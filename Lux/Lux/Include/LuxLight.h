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

		private:
			LightType m_Type;
			vec4 m_Color;

		protected:
			virtual void Reset();
			friend class Core::ComponentFactory;
		};
	}
}

#endif
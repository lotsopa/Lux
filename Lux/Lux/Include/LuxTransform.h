#ifndef LUX_TRANSFORM_H
#define LUX_TRANSFORM_H
#include "LuxComponent.h"

namespace Lux
{
	namespace Core
	{
		class Transform : public Component
		{
		public:
			Transform();
			Transform(const mat4x4& a_TMat);
			~Transform();

			inline const mat4x4& GetMatrix() { return m_TransformMatrix;  }
			void Translate(const vec3& a_Amount);
		private:
			mat4x4 m_TransformMatrix;

		protected:
			void Reset();
			friend class ComponentFactory;
		};
	}
}

#endif
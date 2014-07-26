#ifndef LUX_TRANSFORM_H
#define LUX_TRANSFORM_H
#include "LuxComponent.h"

namespace Lux
{
	class Transform : public Component
	{
	public:
		Transform();
		Transform(const mat4x4& a_TMat);
		~Transform();

	private:
		mat4x4 m_TransformMatrix;
		
	protected:
		void Reset();
		friend class ComponentFactory;
	};
}

#endif
#ifndef LUX_DEFAULT_COMPONENTS_H
#define LUX_DEFAULT_COMPONENTS_H

namespace Lux
{
	/*
	This header file contains all the default components provided by Lux.
	*/
	class Transform : public Component
	{
	public:
		Transform();
		Transform(mat4x4& a_TMatrix);
		~Transform();
		mat4x4 m_TransformMatrix;
	};
}

#endif
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
			~Transform();

			enum SpaceRelativeTo
			{
				SELF = 0,
				WORLD
			};

			inline const mat4x4& GetMatrix() { return m_TransformMatrix;  }

			void ApplyTransform();

			void SetPosition(const vec3& a_Pos);
			const vec3& GetPosition();

			void Translate(const vec3& a_Amount);
			void Rotate(const float a_Angle, const vec3& a_Axis, SpaceRelativeTo a_Space = WORLD);
			void Scale(const vec3& a_Amount);

			void SetScale(const vec3& a_Scale);
			void SetRotation(const quat& a_Rot);
			void SetLocalRotation(const quat& a_Rot);

			const vec3& GetScale();
			const quat& GetRotation();
			const quat& GetLocalRotation() const;
			const vec3& GetForwardVector() const;
			const vec3& GetUpVector() const;
			const vec3& GetRightVector() const;
		private:
			vec3 m_Right;
			vec3 m_Up;
			vec3 m_Forward;

			vec3 m_Position;
			vec3 m_Scale;
			quat m_Rotation;
			quat m_LocalRotation;
			mat4x4 m_TransformMatrix;
			bool m_TransformDirty;

		protected:
			void Reset();
			friend class ComponentFactory;
		};
	}
}

#endif
#ifndef LUX_CAMERA_H
#define LUX_CAMERA_H
#include "LuxComponent.h"

namespace Lux
{
	namespace Graphics
	{
		class Camera : public Core::Component
		{
		public:
			Camera();
			virtual ~Camera();

			void SetProjectionPersp(const float a_FovY, const float a_Aspect, const float a_ZNear, const float a_ZFar);
			void SetProjectionOrtho(const float a_Right, const float a_Left, const float a_Top, const float a_Bottom);
			
			void ApplyViewTransform();

			void SetPosition(const vec3& a_Pos);
			const vec3& GetPosition();

			void Translate(const vec3& a_Amount);
			void Rotate(const float a_Angle, const vec3& a_Axis);
			void Scale(const vec3& a_Amount);

			void SetScale(const vec3& a_Scale);
			void SetRotation(const quat& a_Rot);

			const vec3& GetScale();
			const quat& GetRotation();

			inline void SetMainCamera(const bool a_Val) { m_IsMainCamera = a_Val; }
			inline const bool IsMainCamera() { return m_IsMainCamera; }

			inline const mat4x4& GetViewMatrix() { return m_ViewMatrix; }
			inline const mat4x4& GetProjectionMatrix() { return m_ProjectionMatrix; }

		private:
			mat4x4 m_ViewMatrix;
			mat4x4 m_ProjectionMatrix;
			vec3 m_Translation;
			vec3 m_Scale;
			quat m_Rotation;
			bool m_ViewDirty;
			bool m_IsMainCamera;

		protected:
			void Reset();
			friend class Core::ComponentFactory;
		};
	}
}

#endif
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
			void SetProjectionOrtho(const float a_Right, const float a_Left, const float a_Top, const float a_Bottom, const float a_ZNear, const float a_ZFar);
			void ChangeAspect(const float a_Aspect);
			void SetAsMainCamera();

			inline mat4x4& GetProjectionMatrix() { return m_ProjectionMatrix; }

		private:
			mat4x4 m_ProjectionMatrix;
			float m_ZNear;
			float m_ZFar;
			float m_FovY;
			float m_Aspect;
			float m_Left;
			float m_Right;
			float m_Top;
			float m_Bottom;

		protected:
			void Reset();
			friend class Core::ComponentFactory;
		};
	}
}

#endif
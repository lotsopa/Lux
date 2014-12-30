#ifndef LUX_FREE_LOOK_CAMERA_H
#define LUX_FREE_LOOK_CAMERA_H

namespace Lux
{
	namespace Core
	{
		class FreeLookCamera : public BehaviourComponent
		{
		public:
			FreeLookCamera();
			virtual ~FreeLookCamera();

			virtual void Start();
			virtual void Update(const float a_DeltaTime);
			virtual void OnDestroy();

		private:
			vec2 m_LastMousePos;
			float m_RotationMultiplier;
			float m_SpeedMultiplier;
			vec3 m_EulerRotation;
			ObjectHandle<Transform>* m_Transform;

		protected:
			virtual void Reset();
			friend class ComponentFactory;
		};
	}
}

#endif
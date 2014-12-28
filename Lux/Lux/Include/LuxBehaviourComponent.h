#ifndef LUX_BEHAVIOUR_COMPONENT_H
#define LUX_BEHAVIOUR_COMPONENT_H

namespace Lux
{
	namespace Core
	{
		class Behaviour;
		class EventListener;
		class Transform;

		class BehaviourComponent : public Component
		{
		public:
			BehaviourComponent();
			virtual ~BehaviourComponent();

			virtual void Start() = 0;
			virtual void Update(const float a_DeltaTime) = 0;
			virtual void OnDestroy() = 0;

			inline const bool IsEnabled() const { return m_Enabled; }
			inline void SetEnabled(const bool a_Val) { m_Enabled = a_Val; }
		private:

			struct InitOptions
			{
				EventListener* m_EventListener;
				ObjectHandle<Transform>* m_Transform;
			};

			void Init(InitOptions& a_InitOpt);
			friend class BehaviourSystem;

		protected:
			bool m_Enabled;
			EventListener* m_EventListener;
			ObjectHandle<Transform>* m_Transform;
			virtual void Reset() = 0;
			friend class ComponentFactory;
		};
	}
}

#endif
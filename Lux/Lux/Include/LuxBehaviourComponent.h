#ifndef LUX_BEHAVIOUR_COMPONENT_H
#define LUX_BEHAVIOUR_COMPONENT_H

namespace Lux
{
	namespace Core
	{
		class Behaviour;
		class EventListener;
		class Transform;
		class Key;
		struct ComponentLayout;

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
				InitOptions() : m_EventListener(nullptr)
				{

				}

				~InitOptions()
				{

				}

				EventListener* m_EventListener;
			};

			void Init(InitOptions& a_InitOpt);
			friend class BehaviourSystem;

			const unsigned int GetComponentIndex(const Key& a_Key);
		protected:
			bool m_Enabled;
			EventListener* m_EventListener;
			ComponentLayout* m_ComponentLayout;

			template <class ComponentType>
			__forceinline ObjectHandle<ComponentType>* GetComponent()
			{
				Key k(typeid(ComponentType).name());
				const unsigned int idx = GetComponentIndex(k);
				return (ObjectHandle<ComponentType>*)m_ComponentLayout->m_Components[idx].m_Data;
			}

			virtual void Reset() = 0;
			friend class ComponentFactory;
		};
	}
}

#endif
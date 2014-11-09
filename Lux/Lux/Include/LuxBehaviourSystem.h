#ifndef LUX_BEHAVIOUR_SYSTEM_H
#define LUX_BEHAVIOUR_SYSTEM_H

namespace Lux
{
	namespace Core
	{
		class Entity;
		class BehaviourComponent;

		class BehaviourSystem : public System
		{
		public:
			BehaviourSystem();
			virtual ~BehaviourSystem();

			virtual void ProcessUpdate(const float a_DeltaTime);
			virtual void AddComponent(Component* a_Component, const Key& a_CompType, Entity* a_Entity);
			virtual void RemoveComponent(const Key& a_CompType, Entity* a_Entity);
			virtual bool Init(SceneManager* a_SceneManager);

		private:
			typedef std::map<Entity*, BehaviourComponent*> BehaviourMap;
			BehaviourMap m_BehaviourMap;
		};
	}
}

#endif
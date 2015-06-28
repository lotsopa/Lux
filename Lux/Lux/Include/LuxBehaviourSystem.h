#ifndef LUX_BEHAVIOUR_SYSTEM_H
#define LUX_BEHAVIOUR_SYSTEM_H

namespace Lux
{
	namespace Core
	{
		class Entity;
		class Behaviour;

		class BehaviourSystem : public System
		{
		public:
			BehaviourSystem();
			virtual ~BehaviourSystem();

			virtual void ProcessUpdate(const float a_DeltaTime);
			virtual void AddComponent(void* a_Component, const Key& a_CompType, ObjectHandle<Entity>& a_Entity);
			virtual void RemoveComponent(const Key& a_CompType, ObjectHandle<Entity>& a_Entity);
			virtual bool Init(SceneManager* a_SceneManager);

		private:
			typedef std::map<ObjectHandle<Entity>*, ObjectHandle<Behaviour>*> BehaviourMap;
			BehaviourMap m_BehaviourMap;
		};
	}
}

#endif
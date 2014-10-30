#ifndef LUX_EVENT_SYSTEM_H
#define LUX_EVENT_SYSTEM_H

namespace Lux
{
	namespace Core
	{
		class EventListener;
		class SceneManager;
		class Component;
		class Entity;
		class RenderWindow;
		class Key;

		class EventSystem : public System
		{
		public:
			EventSystem();
			~EventSystem();

			virtual void ProcessUpdate(const float a_DeltaTime);
			virtual bool Init(SceneManager* a_SceneManager);
			virtual void AddComponent(Component* a_Component, const Key& a_CompType, Entity* a_Entity);
			virtual void RemoveComponent(const Key& a_CompType, Entity* a_Entity);

		private:
			RenderWindow* m_RenderWindow;
		};
	}
}

#endif
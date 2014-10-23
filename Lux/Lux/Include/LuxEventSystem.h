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

		class EventSystem : public System
		{
		public:
			EventSystem();
			~EventSystem();

			virtual void ProcessUpdate(const float a_DeltaTime);
			virtual bool Init(SceneManager* a_SceneManager);
			virtual void AddComponent(Component* a_Component, Entity* a_Entity);
			virtual void RemoveComponent(Component* a_Component, Entity* a_Entity);

		private:
			RenderWindow* m_RenderWindow;
		};
	}
}

#endif
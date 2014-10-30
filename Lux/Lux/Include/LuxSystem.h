#ifndef LUX_SYSTEM_H
#define LUX_SYSTEM_H

namespace Lux
{
	namespace Core
	{
		class SceneManager;
		class Component;
		class Entity;
		class Key;
		class System
		{
		public:
			System();
			virtual ~System();

			virtual void ProcessUpdate(const float a_DeltaTime) = 0;
			virtual void AddComponent(Component* a_Component, const Key& a_CompType, Entity* a_Entity) = 0;
			virtual void RemoveComponent(const Key& a_CompType, Entity* a_Entity) = 0;
			virtual bool Init(SceneManager* a_SceneManager);

		protected:
			SceneManager* m_SceneManager;
		};
	}
}

#endif
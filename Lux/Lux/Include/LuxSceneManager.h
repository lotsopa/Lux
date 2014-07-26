#ifndef LUX_SCENE_MANAGER_H
#define LUX_SCENE_MANAGER_H

namespace Lux
{
	class ComponentFactory;
	class EntityFactory;

	class SceneManager
	{
	public:
		SceneManager();
		~SceneManager();

	private:
		ComponentFactory m_ComponentFactory;
		EntityFactory m_EntityFactory;
	};
}

#endif
#ifndef LUX_RENDERING_SYSTEM_H
#define LUX_RENDERING_SYSTEM_H

#include "LuxThreadSafeQueue.h"

namespace Lux
{
	class SceneManager;
	class RenderWindow;
	class MeshRenderer;
	class Transform;

	class RenderingSystem : public System
	{
	public:
		RenderingSystem();
		~RenderingSystem();

		virtual void ProcessUpdate(const float a_DeltaTime);
		virtual bool Init(SceneManager* a_SceneManager);
		virtual void AddComponent(Component* a_Component, Entity* a_Entity);
		virtual void RemoveComponent(Component* a_Component, Entity* a_Entity);
	private:
		RenderWindow* m_RenderWindow;

		bool EntityEntryExists(Entity* a_Entity);

		struct EntityEntry
		{
			EntityEntry() : m_Transform(nullptr), m_MeshRenderer(nullptr)
			{

			}

			~EntityEntry()
			{
				m_Transform = nullptr;
				m_MeshRenderer = nullptr;
			}

			Transform* m_Transform;
			MeshRenderer* m_MeshRenderer;
		};
		typedef std::map<Entity*, EntityEntry> EntityMap;
		EntityMap m_EntityMap;

		typedef ThreadSafeObjectQueue<std::pair<Entity*, Transform*>> TransformEntryQueue;
		typedef ThreadSafeObjectQueue<std::pair<Entity*, MeshRenderer*>> MeshRendererEntryQueue;
		typedef ThreadSafePtrQueue<Entity> EntityQueue;

		TransformEntryQueue m_TransformInsertQueue; // Entries waiting to be inserted 
		EntityQueue m_TransformRemoveQueue; // Entries waiting to be removed

		MeshRendererEntryQueue m_MeshRendererInsertQueue;
		EntityQueue m_MeshRendererRemoveQueue;

		void InsertEntries(); // Inserts entries which are waiting in the queues
		void RemoveEntries(); // Removes entries which are waiting in the queues
	};
}

#endif
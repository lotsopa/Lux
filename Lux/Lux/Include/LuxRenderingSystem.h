#ifndef LUX_RENDERING_SYSTEM_H
#define LUX_RENDERING_SYSTEM_H

#include "LuxThreadSafeQueue.h"

namespace Lux
{
	namespace Core
	{
		class SceneManager;
		class RenderWindow;
		class Transform;
	}


	namespace Graphics
	{
		class MeshRenderer;
		class RenderingSystem : public Core::System
		{
		public:
			RenderingSystem();
			~RenderingSystem();

			virtual void ProcessUpdate(const float a_DeltaTime);
			virtual bool Init(Core::SceneManager* a_SceneManager);
			virtual void AddComponent(Core::Component* a_Component, Core::Entity* a_Entity);
			virtual void RemoveComponent(Core::Component* a_Component, Core::Entity* a_Entity);
		private:
			Core::RenderWindow* m_RenderWindow;

			bool EntityEntryExists(Core::Entity* a_Entity);

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

				Core::Transform* m_Transform;
				MeshRenderer* m_MeshRenderer;
			};
			typedef std::map<Core::Entity*, EntityEntry> EntityMap;
			EntityMap m_EntityMap;

			typedef Utility::ThreadSafeObjectQueue<std::pair<Core::Entity*, Core::Transform*>> TransformEntryQueue;
			typedef Utility::ThreadSafeObjectQueue<std::pair<Core::Entity*, MeshRenderer*>> MeshRendererEntryQueue;
			typedef Utility::ThreadSafePtrQueue<Core::Entity> EntityQueue;

			TransformEntryQueue m_TransformInsertQueue; // Entries waiting to be inserted 
			EntityQueue m_TransformRemoveQueue; // Entries waiting to be removed

			MeshRendererEntryQueue m_MeshRendererInsertQueue;
			EntityQueue m_MeshRendererRemoveQueue;

			void InsertEntries(); // Inserts entries which are waiting in the queues
			void RemoveEntries(); // Removes entries which are waiting in the queues
		};
	}
}

#endif
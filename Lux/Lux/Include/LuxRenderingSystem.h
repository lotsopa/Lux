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
		class Key;
	}


	namespace Graphics
	{
		class MeshRenderer;
		class ShaderComponent;
		class Camera;
		class Light;

		class RenderingSystem : public Core::System
		{
		public:
			RenderingSystem();
			~RenderingSystem();

			virtual void ProcessUpdate(const float a_DeltaTime);
			virtual bool Init(Core::SceneManager* a_SceneManager);
			virtual void AddComponent(Core::Component* a_Component, const Core::Key& a_CompType, Core::Entity* a_Entity) ;
			virtual void RemoveComponent(const Core::Key& a_CompType, Core::Entity* a_Entity);
		private:
			Core::RenderWindow* m_RenderWindow;

			bool EntityEntryExists(Core::Entity* a_Entity);

			void RenderPass();

			struct EntityEntry
			{
				EntityEntry() : m_Transform(nullptr), m_MeshRenderer(nullptr), m_Shader(nullptr), m_Camera(nullptr),
					m_Init(false), m_Light(nullptr)
				{

				}

				~EntityEntry()
				{
					m_Transform = nullptr;
					m_MeshRenderer = nullptr;
					m_Shader = nullptr;
					m_Camera = nullptr;
					m_Light = nullptr;
				}

				inline bool IsNull()
				{
					if (!m_Transform && !m_MeshRenderer && !m_Shader && !m_Camera && !m_Light)
					{
						return true;
					}

					return false;
				}

				Core::Transform* m_Transform;
				MeshRenderer* m_MeshRenderer;
				ShaderComponent* m_Shader;
				Camera* m_Camera;
				Light* m_Light;
				bool m_Init;
			};
			typedef std::map<Core::Entity*, EntityEntry> EntityMap;
			EntityMap m_EntityMap;
			Core::Key m_TransformKey;
			Core::Key m_MeshRendererKey;
			Core::Key m_ShaderKey;
			Core::Key m_CameraKey;
			Core::Key m_LightKey;
			Camera* m_MainCamera;
			EntityEntry* m_LightEntry;
			Core::Transform* m_MainCameraTransform;
		};
	}
}

#endif
#ifndef LUX_RENDERING_SYSTEM_H
#define LUX_RENDERING_SYSTEM_H

#include "LuxThreadSafeQueue.h"
#include "LuxCamera.h"

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
		class Material;

		class RenderingSystem : public Core::System
		{
		public:
			RenderingSystem();
			~RenderingSystem();

			virtual void ProcessUpdate(const float a_DeltaTime);
			virtual bool Init(Core::SceneManager* a_SceneManager);
			virtual void AddComponent(void* a_Component, const Core::Key& a_CompType, Core::ObjectHandle<Core::Entity>& a_Entity);
			virtual void RemoveComponent(const Core::Key& a_CompType, Core::ObjectHandle<Core::Entity>& a_Entity);
		private:
			Core::RenderWindow* m_RenderWindow;

			bool EntityEntryExists(Core::ObjectHandle<Core::Entity>& a_Entity);

			void RenderPass();

			struct EntityEntry
			{
				EntityEntry() : m_Transform(nullptr), m_MeshRenderer(nullptr), m_Material(nullptr), m_Camera(nullptr),
					m_Init(false), m_Light(nullptr)
				{

				}

				~EntityEntry()
				{
					m_Transform = nullptr;
					m_MeshRenderer = nullptr;
					m_Material = nullptr;
					m_Camera = nullptr;
					m_Light = nullptr;
				}

				inline bool IsNull()
				{
					if (!m_Transform && !m_MeshRenderer && !m_Camera && !m_Light && !m_Material)
					{
						return true;
					}

					if (m_Transform)
					{
						if (m_Transform->IsValid())
							return false;
					}

					if (m_MeshRenderer)
					{
						if (m_MeshRenderer->IsValid())
							return false;
					}

					if (m_Material)
					{
						if (m_Material->IsValid())
							return false;
					}

					if (m_Camera)
					{
						if (m_Camera->IsValid())
							return false;
					}

					if (m_Light)
					{
						if (m_Light->IsValid())
							return false;
					}

					return true;
				}

				Core::ObjectHandle<Core::Transform>* m_Transform;
				Core::ObjectHandle<MeshRenderer>* m_MeshRenderer;
				Core::ObjectHandle<Material>* m_Material;
				Core::ObjectHandle<Camera>* m_Camera;
				Core::ObjectHandle<Light>* m_Light;
				bool m_Init;
			};
			typedef std::map<Core::ObjectHandle<Core::Entity>*, EntityEntry> EntityMap;
			typedef std::map<Core::Key, std::function<void(void*, Core::ObjectHandle<Core::Entity>&)>> AddComponentProcessMap;
			typedef std::map<Core::Key, std::function<void(Core::ObjectHandle<Core::Entity>&)>> RemoveComponentProcessMap;

			EntityMap m_EntityMap;
			Core::Key m_TransformKey;
			Core::Key m_MeshRendererKey;
			Core::Key m_ShaderKey;
			Core::Key m_CameraKey;
			Core::Key m_LightKey;
			Core::Key m_MaterialKey;
			Core::ObjectHandle<Camera>* m_MainCamera;
			EntityEntry* m_LightEntry;
			Core::ObjectHandle<Core::Transform>* m_MainCameraTransform;
			Core::ShaderUniformBuffer m_UniformBuffer;
			AddComponentProcessMap m_AddComponentFuncMap;
			RemoveComponentProcessMap m_RemoveComponentProcessMap;

			template<class ComponentType>
			void AddComponentInternal(void* a_CompPtr, Core::ObjectHandle<Core::Entity>& a_Owner)
			{
				Utility::ThrowError("Could not add Component to Rendering System. Not supported.");
			}

			// Specializations
			template<> void AddComponentInternal<Core::Transform>(void* a_CompPtr, Core::ObjectHandle<Core::Entity>& a_Owner)
			{
				m_EntityMap[&a_Owner].m_Transform = (Core::ObjectHandle<Lux::Core::Transform>*)(a_CompPtr);
			}

			template<> void AddComponentInternal<MeshRenderer>(void* a_CompPtr, Core::ObjectHandle<Core::Entity>& a_Owner)
			{
				m_EntityMap[&a_Owner].m_MeshRenderer = (Core::ObjectHandle<MeshRenderer>*)(a_CompPtr);

				if (m_EntityMap[&a_Owner].m_Material)
				{
					m_EntityMap[&a_Owner].m_Init = false;
				}
			}

			template<> void AddComponentInternal<Material>(void* a_CompPtr, Core::ObjectHandle<Core::Entity>& a_Owner)
			{
				m_EntityMap[&a_Owner].m_Material = (Core::ObjectHandle<Material>*)(a_CompPtr);

				if (m_EntityMap[&a_Owner].m_MeshRenderer)
				{
					m_EntityMap[&a_Owner].m_Init = false;
				}
			}

			template<> void AddComponentInternal<Camera>(void* a_CompPtr, Core::ObjectHandle<Core::Entity>& a_Owner)
			{
				m_EntityMap[&a_Owner].m_Camera = (Core::ObjectHandle<Camera>*)(a_CompPtr);

				if (m_EntityMap[&a_Owner].m_Camera->GetRawPtr()->IsMainCamera())
				{
					m_MainCamera = m_EntityMap[&a_Owner].m_Camera;
				}
			}

			template<> void AddComponentInternal<Light>(void* a_CompPtr, Core::ObjectHandle<Core::Entity>& a_Owner)
			{
				m_EntityMap[&a_Owner].m_Light = (Core::ObjectHandle<Light>*)(a_CompPtr);
				m_LightEntry = &m_EntityMap[&a_Owner];
			}

			template<class ComponentType>
			void RemoveComponentInternal(Core::ObjectHandle<Core::Entity>& a_Owner)
			{
				Utility::ThrowError("Could not add Component to Rendering System. Not supported.");
			}

			// Specializations
			template<> void RemoveComponentInternal<Core::Transform>(Core::ObjectHandle<Core::Entity>& a_Owner)
			{
				m_EntityMap[&a_Owner].m_Transform = nullptr;
			}

			template<> void RemoveComponentInternal<MeshRenderer>(Core::ObjectHandle<Core::Entity>& a_Owner)
			{
				m_EntityMap[&a_Owner].m_MeshRenderer = nullptr;
			}

			template<> void RemoveComponentInternal<ShaderComponent>(Core::ObjectHandle<Core::Entity>& a_Owner)
			{
				m_EntityMap[&a_Owner].m_Material = nullptr;
			}

			template<> void RemoveComponentInternal<Camera>(Core::ObjectHandle<Core::Entity>& a_Owner)
			{
				if (m_EntityMap[&a_Owner].m_Camera->GetRawPtr()->IsMainCamera())
					m_MainCamera = nullptr;

				m_EntityMap[&a_Owner].m_Camera = nullptr;
			}

			template<> void RemoveComponentInternal<Light>(Core::ObjectHandle<Core::Entity>& a_Owner)
			{
				m_EntityMap[&a_Owner].m_Light = nullptr;
			}

			template<> void RemoveComponentInternal<Material>(Core::ObjectHandle<Core::Entity>& a_Owner)
			{
				m_EntityMap[&a_Owner].m_Material = nullptr;
			}

		};
	}
}

#endif
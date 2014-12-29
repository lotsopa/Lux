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
		class MaterialComponent;

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
				EntityEntry() : m_Transform(nullptr), m_MeshRenderer(nullptr), m_Shader(nullptr), m_Camera(nullptr),
					m_Init(false), m_Light(nullptr), m_Material(nullptr)
				{

				}

				~EntityEntry()
				{
					m_Transform = nullptr;
					m_MeshRenderer = nullptr;
					m_Shader = nullptr;
					m_Camera = nullptr;
					m_Light = nullptr;
					m_Material = nullptr;
				}

				inline bool IsNull()
				{
					if (!m_Transform && !m_MeshRenderer && !m_Shader && !m_Camera && !m_Light && !m_Material)
					{
						return true;
					}

					if (m_Transform)
					{
						if (!m_Transform->IsValid())
							return false;
					}

					if (m_MeshRenderer)
					{
						if (!m_MeshRenderer->IsValid())
							return false;
					}

					if (m_Shader)
					{
						if (!m_Shader->IsValid())
							return false;
					}

					if (m_Material)
					{
						if (!m_Material->IsValid())
							return false;
					}

					if (m_Camera)
					{
						if (!m_Camera->IsValid())
							return false;
					}

					if (m_Light)
					{
						if (!m_Light->IsValid())
							return false;
					}

					return false;
				}

				Core::ObjectHandle<Core::Transform>* m_Transform;
				Core::ObjectHandle<MeshRenderer>* m_MeshRenderer;
				Core::ObjectHandle<ShaderComponent>* m_Shader;
				Core::ObjectHandle<MaterialComponent>* m_Material;
				Core::ObjectHandle<Camera>* m_Camera;
				Core::ObjectHandle<Light>* m_Light;
				bool m_Init;
			};
			typedef std::map<Core::ObjectHandle<Core::Entity>*, EntityEntry> EntityMap;
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
			mat4x4 m_ModelViewProj;
		};
	}
}

#endif
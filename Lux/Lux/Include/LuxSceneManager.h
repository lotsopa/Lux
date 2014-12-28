#ifndef LUX_SCENE_MANAGER_H
#define LUX_SCENE_MANAGER_H

namespace Lux
{
	namespace Core
	{
		class ComponentFactory;
		class SystemFactory;
		class EntityFactory;
		class Entity;
		class System;
		class RenderWindow;

		class SceneManager
		{
		public:
			SceneManager(RenderWindow* a_RenderWindow);
			~SceneManager();

			/*
			Creates an Entity.
			Returns the created entity pointer on success, NULL otherwise.
			*/
			ObjectHandle<Entity>& CreateEntity();

			/*
			Destroys the given entity and sets the passed pointer to NULL.
			Returns true on success, false otherwise.
			Throws an exception if the passed Entity pointer is NULL.
			*/
			bool DestroyEntity(ObjectHandle<Lux::Core::Entity>& a_Ent);

			/*
			Registers a new component type with the system. Note that this function does not create any actual components.
			It only notifies the system that a component of this type will be used during the program execution.
			Registering component types MUST be done before the first entity is created in the program flow.
			Registering new component types after even one entity has been created may lead to undefined behaviour.
			*/
			template<class ComponentType>
			void RegisterNewComponentType(unsigned int a_InitialSize = LUX_COMPONENT_MEMORY_POOL_INITIAL_SIZE, unsigned int a_GrowAmount = LUX_COMPONENT_MEMORY_POOL_GROW_AMOUNT)
			{
				Key k(typeid(ComponentType).name());
				m_ComponentFactory->CreateComponentType<ComponentType>(a_InitialSize, a_GrowAmount);
				m_ComponentIndexMap.insert(std::make_pair(k, m_NumComponentTypes));
				m_DelFuncMap.insert(std::make_pair(m_NumComponentTypes, std::bind(&SceneManager::DestroyComponent<ComponentType>, this, std::placeholders::_1, std::placeholders::_2)));
				m_NumComponentTypes++;
			}

			/*
			Gets a Component of type T from an entity. Returns NULL if the entity does not have a component of this type attached.
			Throws an exception if the passed Entity pointer is NULL or the component type is not registered.
			*/
			template<class ComponentType>
			ObjectHandle<ComponentType>&  GetComponent(ObjectHandle<Entity>& a_Ent)
			{
				ComponentLayout& layout = m_EntityComponentMap.at(&a_Ent);
				unsigned int idx = m_ComponentIndexMap.at(Key(typeid(ComponentType).name()));

				ObjectHandle<ComponentType>& retVal = *(ObjectHandle<ComponentType>*)layout.m_Components[idx].m_Data;
				return retVal;
			}

			/*
			Attaches a new component of type T to the specified entity.
			Returns NULL if the entity already has a component of this type attached.
			Throws an exception if the passed Entity pointer is NULL or the component type is unregistered.
			*/
			template <class ComponentType>
			ObjectHandle<ComponentType>& AttachNewComponent(ObjectHandle<Entity>& a_Ent)
			{
				bool hasComponent = HasComponent<ComponentType>(a_Ent);

				if (hasComponent)
				{
					Utility::ThrowError("A component of that type is already attached to the entity.");
				}

				ObjectHandle<ComponentType>& comp = m_ComponentFactory->CreateComponent<ComponentType>();
				ComponentLayout& layout = m_EntityComponentMap.at(&a_Ent);
				Key k(typeid(ComponentType).name());
				unsigned int idx = m_ComponentIndexMap.at(k);
				layout.m_Components[idx].m_Data = &comp;

				// Notify all Systems that are registered for this component type
				std::pair<ComponentSystemMultiMap::iterator, ComponentSystemMultiMap::iterator> ret;
				ret = m_ComponentSystemMap.equal_range(k);
				ComponentSystemMultiMap::iterator iter;
				for (iter = ret.first; iter != ret.second; ++iter)
				{
					iter->second->AddComponent(&comp, k, a_Ent);
				}

				return comp;
			}

			/*
			Checks if the specified entity has a component of type T attached to it.
			Returns true if there is a component of this type attached to the entity, false otherwise.
			Throws an exception if the passed Entity pointer is NULL.
			*/
			template<class ComponentType>
			bool HasComponent(ObjectHandle<Entity>& a_Ent)
			{
				ComponentLayout& layout = m_EntityComponentMap.at(&a_Ent);
				Key k(typeid(ComponentType).name());
				unsigned int idx = m_ComponentIndexMap.at(k);
				if (!layout.m_Components[idx].m_Data)
				{
					return false;
				}
				else
				{
					ObjectHandle<ComponentType>* comp = (ObjectHandle<ComponentType>*)layout.m_Components[idx].m_Data;
					if (!comp->IsValid())
						return false;
				}

				return true;
			}

			/*
			Detaches a component of type T from the specified entity.
			Throws an exception if the passed Entity pointer is NULL or the component type is unregistered.
			*/
			template<class ComponentType>
			bool DetachComponent(ObjectHandle<Entity>& a_Ent)
			{
				ComponentLayout& layout = m_EntityComponentMap.at(&a_Ent);
				Key k(typeid(ComponentType).name());
				unsigned int idx = m_ComponentIndexMap.at(k);

				if (!layout.m_Components[idx].m_Data)
				{
					return false;
				}
				else
				{
					ObjectHandle<ComponentType>* comp = (ObjectHandle<ComponentType>*)layout.m_Components[idx].m_Data;
					if (!comp->IsValid())
						return false;
				}

				DestroyComponent<ComponentType>(layout.m_Components[idx].m_Data, a_Ent);
				return true;
			}

			/*
			Registers a new type of System and notifies the program of its existence.
			After registering a new system type, one instance of the that type will be automatically created.
			That's the reason there is no Create Function for the Systems.
			More than one system of each type is not allowed for obvious reasons.
			*/
			template<class SystemType>
			void RegisterNewSystemType()
			{
				m_SystemFactory->AddSystemTypeToFactory<SystemType>();
				System* sys = m_SystemFactory->InstantiateNewSystem<SystemType>();
				bool res = sys->Init(this);

				if (!res)
				{
					Utility::ThrowError("Could not initialize system.");
				}
				Key k(typeid(SystemType).name());
				m_SystemsMap.insert(std::make_pair(k, sys));
			}

			/*
			Registers the Component type with the system type.
			This notifies the program which kind of components it should forward to each system after they are created.
			For example: When a Transform component is created, all systems which operate on a Transform type
			will be automatically notified of its creation so they can operate on it.
			*/
			template<class ComponentType, class SystemType>
			void RegisterComponentTypeWithSystem()
			{
				Key k(typeid(SystemType).name());
				System* system = m_SystemsMap.at(k);
				Key compKey(typeid(ComponentType).name());
				m_ComponentSystemMap.insert(std::make_pair(compKey, system));
			}

			void ProcessUpdate(const float a_Dt);

			inline RenderWindow* GetRenderWindow() { return m_RenderWindow; }

		private:

			template<class ComponentType>
			void DestroyComponent(void* a_Comp, ObjectHandle<Entity>& a_Entity)
			{
				Key k(typeid(ComponentType).name());
				ObjectHandle<ComponentType>* comp = (ObjectHandle<ComponentType>*)a_Comp;
				// Notify all Systems that are registered for this component type
				std::pair<ComponentSystemMultiMap::iterator, ComponentSystemMultiMap::iterator> ret;
				ret = m_ComponentSystemMap.equal_range(k);
				ComponentSystemMultiMap::iterator iter;
				for (iter = ret.first; iter != ret.second; ++iter)
				{
					iter->second->RemoveComponent(k, a_Entity);
				}

				if (m_ComponentFactory->DestroyComponent(*comp))
				{
					comp->m_RawPtr = nullptr;
					a_Comp = nullptr;
				}
				else
				{
					// Normally this should never happen, but just in case
					Utility::ThrowError("Could not destroy attached component.");
				}
			}

			struct ComponentLayout
			{
				ComponentLayout(const unsigned int a_NumComponents, ObjectHandle<Entity>& a_Owner, SceneManager* a_Manager) :
					m_NumComponents(a_NumComponents), m_Owner(a_Owner), m_SceneManager(a_Manager)
				{
					m_Components = new ComponentEntry[m_NumComponents];
				}

				ComponentLayout(const ComponentLayout& a_Layout) : m_Owner(a_Layout.m_Owner)
				{
					m_NumComponents = a_Layout.m_NumComponents;
					m_SceneManager = a_Layout.m_SceneManager;
					m_Components = new ComponentEntry[m_NumComponents];
					for (unsigned int i = 0; i < m_NumComponents; i++)
					{
						m_Components[i].m_Data = a_Layout.m_Components->m_Data;
					}
				}

				~ComponentLayout()
				{
					SceneManager::ComponentDelFuncMap::iterator it;

					unsigned int ctr = 0;
					for (it = m_SceneManager->m_DelFuncMap.begin(); it != m_SceneManager->m_DelFuncMap.end(); ++it)
					{
						if (m_Components[it->first].m_Data != nullptr)
						{
							it->second(m_Components[it->first].m_Data, m_Owner);
						}
					}

					Utility::SafeArrayDelete(m_Components);
				}
				struct ComponentEntry
				{
					ComponentEntry() : m_Data(nullptr)
					{
						
					}

					~ComponentEntry()
					{
						m_Data = nullptr;
					}

					void* m_Data;
				};
				ComponentEntry* m_Components;
				unsigned int m_NumComponents;
				SceneManager* m_SceneManager;
				ObjectHandle<Entity>& m_Owner;
			};

			void ProcessSystems(const float a_DeltaTime);

			ComponentFactory* m_ComponentFactory;
			EntityFactory* m_EntityFactory;
			SystemFactory* m_SystemFactory;
			typedef std::map<ObjectHandle<Entity>*, ComponentLayout> EntityComponentMap;
			typedef std::map<Key, unsigned int> ComponentIndexMap;
			typedef std::map<unsigned int, std::function<void(void*, ObjectHandle<Entity>&)>> ComponentDelFuncMap;
			typedef std::map<Key, System*> SystemsMap;
			typedef std::multimap<Key, System*> ComponentSystemMultiMap;
			EntityComponentMap m_EntityComponentMap;
			ComponentIndexMap m_ComponentIndexMap;
			ComponentDelFuncMap m_DelFuncMap;
			unsigned int m_NumComponentTypes;
			SystemsMap m_SystemsMap;
			ComponentSystemMultiMap m_ComponentSystemMap;
			RenderWindow* m_RenderWindow;
		};
	}
}

#endif
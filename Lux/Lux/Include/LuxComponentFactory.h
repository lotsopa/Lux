#ifndef LUX_COMPONENT_FACTORY_H
#define LUX_COMPONENT_FACTORY_H

namespace Lux
{
	class Component;
	class MeshRenderer;
	class Transfrom;
	class ComponentFactory
	{
	public:
		ComponentFactory();
		~ComponentFactory();

		/*
		This function creates a new component instance from the provided component type.
		*/
		template<class T>
		T* CreateComponent()
		{
			ObjectPool<T>* thisPool = (ObjectPool<T>*)m_ComponentPoolMap.at(Key(typeid(T).name()));
			T* objPtr = thisPool->CreateObject();
			objPtr->Reset();
			return objPtr;
		}

		/*
		This function destroys a component instance from the provided component type.
		Returns true if the component was destroyed successfully, false otherwise.
		*/
		template<class T>
		bool DestroyComponent(T* a_Component)
		{
			ObjectPool<T>* thisPool = (ObjectPool<T>*)m_ComponentPoolMap.at(Key(typeid(T).name()));
			return thisPool->DeleteObject(a_Component);
		}

		/*
		Using this function, it is possible to create a new component type.
		By calling this function the factory will create a new pool for this type of component.
		This function does NOT create an actual object of the type.
		If you want to create an instance of this type of component, use the function CreateComponent().
		*/
		template<class T>
		void CreateComponentType(const unsigned int a_InitialPoolSize, const unsigned int a_PoolGrowSize)
		{
			// Make a key from the class name
			Key k(typeid(T).name());

			// Create an entry inside the Component Pool map and register a delete function for this component type
			m_ComponentPoolMap.insert(std::make_pair(k, new ObjectPool<T>(a_InitialPoolSize, a_PoolGrowSize)));
			m_ComponentDeleteMap.insert(std::make_pair(k, std::bind(&ComponentFactory::DeletePool<T>, this)));
		}

		template<class T>
		bool ComponentTypeExists()
		{
			// Make a key from the class name
			Key k(typeid(T).name());

			unsigned int count = m_ComponentPoolMap.count(k);

			if (count > 0)
			{
				return true;
			}

			return false;
		}

	private:

		/*
		Private function to clean up a created component pool.
		*/
		template<class T>
		void DeletePool()
		{
			Key k(typeid(T).name());
			ObjectPool<T>* thisPool = (ObjectPool<T>*)m_ComponentPoolMap.at(k);
			SafePtrDelete(thisPool);
			m_ComponentPoolMap[k] = nullptr;
		}

		typedef std::map<Key, std::function<void(void)>> ComponentDelFuncMap;
		typedef std::map<Key, void*> ComponentPoolMap;
		ComponentPoolMap m_ComponentPoolMap;
		ComponentDelFuncMap m_ComponentDeleteMap;
	};
}

#endif
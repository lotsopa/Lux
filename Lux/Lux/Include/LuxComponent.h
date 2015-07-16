#ifndef LUX_COMPONENT_H
#define LUX_COMPONENT_H
#include "LuxMessageManager.h"
#include "LuxObjectPool.h"
#include "LuxEntity.h"

namespace Lux
{
	namespace Core
	{
		/*
		Base class for all entity components.
		*/
		class Component
		{
		public:
			virtual ~Component();

		protected:
			Component();

			// Reset the Component to a pristine state
			virtual void Reset() = 0;
			template <class T>
			friend class ObjectPool;
			friend class ComponentFactory;
			friend class SceneManager;
			MessageManager* m_MessageManager;
			ObjectHandle<Entity>* m_Owner;
		};
	}
}

#endif
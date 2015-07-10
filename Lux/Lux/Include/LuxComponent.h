#ifndef LUX_COMPONENT_H
#define LUX_COMPONENT_H

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
			friend class ResourceHandler;
		};
	}
}

#endif
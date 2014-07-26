#ifndef LUX_COMPONENT_H
#define LUX_COMPONENT_H

namespace Lux
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

		unsigned int m_Index;
		template <class T>
		friend class ObjectPool;
		friend class ComponentFactory;
		friend class ResourceHandler;
	};
}

#endif
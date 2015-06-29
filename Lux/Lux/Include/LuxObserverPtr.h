#ifndef LUX_OBSERVER_PTR_H
#define LUX_OBSERVER_PTR_H

/*
A Simple Implementation of an observer pointer. 
The observer pointer does not own the data to which it is pointing.
It does not have permission to delete it.
When release() is called, the object simply loses the reference to the pointed data, without destroying it.
*/
namespace Lux
{
	namespace Core
	{
		template<typename T>
		struct nop_deleter
		{
			void operator()(T*) const {}
		};

		template<typename T>
		using ObserverPtr = std::unique_ptr < T, nop_deleter<T> > ;
	}
}

#endif
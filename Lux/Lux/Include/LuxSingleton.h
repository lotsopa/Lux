#ifndef LUX_SINGLETON_H
#define LUX_SINGLETON_H

namespace Lux
{
	template <class T>
	class Singleton
	{
	public:
		static T& GetInstance()
		{
			static T instance;
			return instance;
		}
		virtual ~Singleton() {};

	protected:
		inline explicit Singleton() {};
		Singleton(Singleton const&);// Don't Implement
		void operator=(Singleton const&);// Don't implement
	};
}

#endif
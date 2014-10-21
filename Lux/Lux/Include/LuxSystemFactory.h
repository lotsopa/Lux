#ifndef LUX_SYSTEM_FACTORY_H
#define LUX_SYSTEM_FACTORY_H

namespace Lux
{
	namespace Core
	{
		class System;
		class SystemFactory
		{
		public:
			SystemFactory();
			~SystemFactory();

			template<class T>
			void AddSystemTypeToFactory()
			{
				Key key(typeid(T).name());
				m_SystemTypeMap.insert(std::make_pair(key, &CreateClassInstance<T>));
			}

			template<class T>
			System* InstantiateNewSystem()
			{
				Key key(typeid(T).name());
				return m_SystemTypeMap.at(key)();
			}

			template<class T>
			bool SystemTypeExists()
			{
				Key key(typeid(T).name());
				int count = m_SystemTypeMap.count(key);

				if (count > 0)
				{
					return true;
				}

				return false;
			}

		private:

			template<typename T>
			static System* CreateClassInstance()
			{
				return new T;
			}

			typedef std::map<Key, System*(*)()> SystemConstructorMap;
			SystemConstructorMap m_SystemTypeMap;
		};
	}
}

#endif
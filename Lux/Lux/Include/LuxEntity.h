#ifndef LUX_ENTITY_H
#define LUX_ENTITY_H

namespace Lux
{
	namespace Core
	{
		class Component;
		class Model;
		class Transform;
		/*
		The Entity class is just an abstract concept. It represents an object in the scene.
		*/
		class Entity
		{
		public:
			~Entity();

		private:
			Entity();
			unsigned int m_Index;
			void Reset();
			template <class T>
			friend class ObjectPool;

			friend class EntityFactory;
		};
	}
}

#endif
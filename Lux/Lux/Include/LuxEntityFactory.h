#ifndef ENTITY_FACTORY_H
#define ENTITY_FACTORY_H

namespace Lux
{
	namespace Core
	{
		/*
		The Entity Factory is responsible for creating and destroying entities
		and also keeps a list of all currently available entities.
		*/
		class EntityFactory
		{
		public:
			EntityFactory(const unsigned int a_InitialPoolSize, const unsigned int a_GrowSize);
			~EntityFactory();

			ObjectHandle<Entity>& CreateEntity();
			bool DestroyEntity(ObjectHandle<Entity>& a_Entity);

		private:
			ObjectPool<Entity> m_EntityMemoryPool;
		};
	}
}

#endif
#ifndef LUX_ENTITY_H
#define LUX_ENTITY_H

namespace Lux
{
	class Mesh;
	class EntityAnimation;
	// An entity can be composed of one or more meshes.
	class Entity
	{
	public:
		Entity(unsigned int a_NumMeshes, unsigned int a_NumAnims);
		~Entity();

	private:
		Mesh** m_Meshes;
		EntityAnimation** m_AnimationData;
		unsigned int m_NumMeshes;
		unsigned int m_NumAnimations;
		void AddMesh(Mesh* a_Mesh);
		void AddAnimation(EntityAnimation* a_Anim);
		friend class ResourceHandler;
	};
}

#endif
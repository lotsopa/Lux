#ifndef LUX_ENTITY_H
#define LUX_ENTITY_H

namespace Lux
{
	class Mesh;

	// An entity can be composed of one or more meshes.
	class Entity
	{
	public:
		Entity(unsigned int a_NumMeshes);
		~Entity();

	private:
		Mesh** m_Meshes;
		unsigned int m_NumMeshes;
		void AddMesh(Mesh* a_Mesh);
		friend class ResourceHandler;
	};
}

#endif
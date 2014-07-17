#ifndef LUX_MESH_H
#define LUX_MESH_H

namespace Lux
{
	class SubMesh;
	class MeshAnimation;
	// A mesh can be composed of one or more sub meshes.
	class Mesh
	{
	public:
		Mesh(unsigned int a_NumMeshes, unsigned int a_NumAnims);
		~Mesh();

	private:
		SubMesh** m_Meshes;
		MeshAnimation** m_AnimationData;
		unsigned int m_NumMeshes;
		unsigned int m_NumAnimations;
		void AddMesh(SubMesh* a_Mesh);
		void AddAnimation(MeshAnimation* a_Anim);
		friend class ResourceHandler;
	};
}

#endif
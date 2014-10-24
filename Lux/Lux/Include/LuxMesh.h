#ifndef LUX_MESH_H
#define LUX_MESH_H

namespace Lux
{
	namespace Core
	{
		class SubMesh;
		class MeshAnimation;

		/*
		Mesh component which can be added to an entity.
		A mesh can be composed of one or more sub meshes.
		*/
		class Mesh
		{
		public:
			Mesh();
			Mesh(const Mesh& a_Mesh);
			Mesh(unsigned int a_NumMeshes, unsigned int a_NumAnims);
			~Mesh();

			void AddSubMesh(SubMesh* a_Mesh);
			void AddAnimation(MeshAnimation* a_Anim);

		private:
			SubMesh** m_SubMeshes;
			MeshAnimation** m_AnimationData;
			unsigned int m_NumSubMeshes;
			unsigned int m_NumAnimations;
		};
	}
}

#endif
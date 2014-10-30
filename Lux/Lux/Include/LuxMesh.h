#ifndef LUX_MESH_H
#define LUX_MESH_H

namespace Lux
{
	namespace Core
	{
		class SubMesh;
		class MeshAnimation;

		/*
		Mesh class.
		A mesh can be composed of one or more sub meshes.
		*/
		class Mesh
		{
		public:
			Mesh();
			virtual ~Mesh();

			virtual void AddSubMesh(SubMesh* a_Mesh) = 0;
			virtual void AddAnimation(MeshAnimation* a_Anim) = 0;
		};
	}
}

#endif
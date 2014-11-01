#ifndef LUX_MESH_H
#define LUX_MESH_H

namespace Lux
{
	namespace Core
	{
		class SubMesh;
		class MeshAnimation;
		class Shader;
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
			virtual void ConnectWithShader(Shader* a_Shader) = 0;

			virtual const unsigned int GetNumSubMeshes() = 0;
			virtual SubMesh* GetSubMesh(const unsigned int a_Index) = 0;
		};
	}
}

#endif
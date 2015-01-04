#ifndef LUX_MESH_DX11_H
#define LUX_MESH_DX11_H

namespace Lux
{
	namespace Core
	{
		class SubMesh;
		class MeshAnimation;
		class Shader;
		namespace Internal
		{
			class MeshDX11 : public Mesh
			{
			public:
				MeshDX11();
				virtual ~MeshDX11();

				virtual void AddSubMesh(SubMesh* a_Mesh);
				virtual void AddAnimation(MeshAnimation* a_Anim);
				virtual void ConnectWithShader(Shader* a_Shader);

				virtual const unsigned int GetNumSubMeshes();
				virtual SubMesh* GetSubMesh(const unsigned int a_Index);
			};
		}
	}
}

#endif
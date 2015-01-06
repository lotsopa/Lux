#ifndef LUX_MESH_DX11_H
#define LUX_MESH_DX11_H

namespace Lux
{
	namespace Core
	{
		class SubMesh;
		class MeshAnimation;
		class Shader;
		class SubMeshDX11;
		namespace Internal
		{
			class MeshDX11 : public Mesh
			{
			public:
				MeshDX11();
				MeshDX11(unsigned int a_NumMeshes, unsigned int a_NumAnims);
				virtual ~MeshDX11();

				virtual void AddSubMesh(SubMesh* a_Mesh);
				virtual void AddAnimation(MeshAnimation* a_Anim);
				virtual void ConnectWithShader(Shader* a_Shader);

				virtual const unsigned int GetNumSubMeshes();
				virtual SubMesh* GetSubMesh(const unsigned int a_Index);

			private:
				SubMeshDX11** m_SubMeshes;
				MeshAnimation** m_AnimationData;
				unsigned int m_NumSubMeshes;
				unsigned int m_NumAnimations;
			};
		}
	}
}

#endif
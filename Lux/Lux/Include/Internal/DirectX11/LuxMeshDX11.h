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

				virtual const unsigned int GetNumSubMeshes();
				virtual ObserverPtr<SubMesh> GetSubMesh(const unsigned int a_Index);

				// Sets the material properties for all the sub meshes
				virtual void SetMaterialProperties(Core::ObserverPtr<Material>& a_Mat);

				// Sets the diffuse texture for all the sub meshes
				virtual void SetTexture(TextureIndex a_Idx, Core::ObserverPtr<Texture2D>& a_Tex);

				// Sets the shader for all the sub meshes
				virtual void SetShader(ObserverPtr<Shader>& a_Shader);

			private:
				SubMeshDX11** m_SubMeshes;
				MeshAnimation** m_AnimationData;
				unsigned int m_NumSubMeshes;
				unsigned int m_NumAnimations;
				unsigned int m_SubMeshCtr;
				unsigned int m_AnimCtr;
			};
		}
	}
}

#endif
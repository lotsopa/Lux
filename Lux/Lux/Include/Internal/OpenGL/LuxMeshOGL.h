#ifndef LUX_MESH_OGL_H
#define LUX_MESH_OGL_H

namespace Lux
{
	namespace Core
	{
		namespace Internal
		{
			class MeshOGL : public Mesh
			{
			public:
				MeshOGL();
				MeshOGL(unsigned int a_NumMeshes, unsigned int a_NumAnims);
				virtual ~MeshOGL();
				
				void AddSubMesh(SubMesh* a_Mesh);
				void AddAnimation(MeshAnimation* a_Anim) ;
				virtual const unsigned int GetNumSubMeshes();
				virtual ObserverPtr<SubMesh> GetSubMesh(const unsigned int a_Index);

				// Sets the material properties for all the sub meshes
				virtual void SetMaterialProperties(Core::ObserverPtr<Material>& a_Mat);

				// Sets the diffuse texture for all the sub meshes
				virtual void SetTexture(TextureIndex a_Idx, Core::ObserverPtr<Texture2D>& a_Tex);

				// Sets the shader for all the sub meshes
				virtual void SetShader(ObserverPtr<Shader>& a_Shader);

			private:
				SubMeshOGL** m_SubMeshes;
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
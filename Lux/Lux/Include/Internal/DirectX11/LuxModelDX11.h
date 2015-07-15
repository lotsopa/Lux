#ifndef LUX_MODEL_DX11_H
#define LUX_MODEL_DX11_H

namespace Lux
{
	namespace Core
	{
		class Mesh;
		class MeshAnimation;
		class Shader;
		class MeshDX11;
		namespace Internal
		{
			class ModelDX11 : public Model
			{
			public:
				ModelDX11();
				ModelDX11(unsigned int a_NumMeshes, unsigned int a_NumAnims);
				virtual ~ModelDX11();

				virtual void AddMesh(Mesh* a_Mesh);
				virtual void AddAnimation(MeshAnimation* a_Anim);

				virtual const unsigned int GetNumMeshes();
				virtual ObserverPtr<Mesh> GetMesh(const unsigned int a_Index);

				// Sets the material properties for all the sub meshes
				virtual void SetMaterialProperties(Core::ObserverPtr<Material>& a_Mat);

				// Sets the diffuse texture for all the sub meshes
				virtual void SetTexture(TextureIndex a_Idx, Core::ObserverPtr<Texture2D>& a_Tex);

				// Sets the shader for all the sub meshes
				virtual void SetShader(ObserverPtr<Shader>& a_Shader);

			private:
				MeshDX11** m_SubMeshes;
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
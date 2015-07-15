#ifndef LUX_MODEL_OGL_H
#define LUX_MODEL_OGL_H

namespace Lux
{
	namespace Core
	{
		class MeshOGL;
		namespace Internal
		{
			class ModelOGL : public Model
			{
			public:
				ModelOGL();
				ModelOGL(unsigned int a_NumMeshes, unsigned int a_NumAnims);
				virtual ~ModelOGL();
				
				void AddMesh(Mesh* a_Mesh);
				void AddAnimation(MeshAnimation* a_Anim) ;
				virtual const unsigned int GetNumMeshes();
				virtual ObserverPtr<Mesh> GetMesh(const unsigned int a_Index);

				// Sets the material properties for all the sub meshes
				virtual void SetMaterialProperties(Core::ObserverPtr<Material>& a_Mat);

				// Sets the diffuse texture for all the sub meshes
				virtual void SetTexture(TextureIndex a_Idx, Core::ObserverPtr<Texture2D>& a_Tex);

				// Sets the shader for all the sub meshes
				virtual void SetShader(ObserverPtr<Shader>& a_Shader);

			private:
				MeshOGL** m_Meshes;
				MeshAnimation** m_AnimationData;
				unsigned int m_NumMeshes;
				unsigned int m_NumAnimations;
				unsigned int m_MeshCtr;
				unsigned int m_AnimCtr;
			};
		}
	}
}

#endif
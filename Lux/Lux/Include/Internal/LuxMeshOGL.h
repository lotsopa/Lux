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
				virtual void ConnectWithShader(Shader* a_Shader);
				virtual const unsigned int GetNumSubMeshes();
				virtual SubMesh* GetSubMesh(const unsigned int a_Index);
			private:
				SubMeshOGL** m_SubMeshes;
				MeshAnimation** m_AnimationData;
				unsigned int m_NumSubMeshes;
				unsigned int m_NumAnimations;
			};
		}
	}
}

#endif
#ifndef LUX_SUB_MESH_OGL_H
#define LUX_SUB_MESH_OGL_H

namespace Lux
{
	namespace Core
	{
		namespace Internal
		{
			class ShaderOGL;

			class SubMeshOGL : public SubMesh
			{
			public:
				SubMeshOGL();
				SubMeshOGL(const SubMeshOGL& a_SubMesh);
				SubMeshOGL(aiMesh& a_Mesh);
				virtual ~SubMeshOGL();

				virtual void PreRender();
				virtual void PostRender();

				void ConnectWithShader(ShaderOGL* a_Shader);
			protected:
				BufferOGL m_VertexBuffer;
				BufferOGL m_IndexBuffer;
				VertexArrayOGL m_VertexArray;
			};
		}
	}
}

#endif
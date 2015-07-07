#ifndef LUX_MESH_OGL_H
#define LUX_MESH_OGL_H

namespace Lux
{
	namespace Core
	{
		namespace Internal
		{
			class ShaderOGL;

			class MeshOGL : public Mesh
			{
			public:
				MeshOGL();
				MeshOGL(const MeshOGL& a_SubMesh);
				MeshOGL(aiMesh& a_Mesh);
				virtual ~MeshOGL();

				virtual void PreRender();
				virtual void PostRender();
			protected:
				virtual void ConnectWithShader(Shader* a_Shader);
				BufferOGL m_VertexBuffer;
				BufferOGL m_IndexBuffer;
				VertexArrayOGL m_VertexArray;
			};
		}
	}
}

#endif
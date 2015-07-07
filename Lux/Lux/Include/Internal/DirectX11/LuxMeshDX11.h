#ifndef LUX_MESH_DX11_H
#define LUX_MESH_DX11_H

namespace Lux
{
	namespace Core
	{
		namespace Internal
		{
			class MeshDX11 : public Mesh
			{
			public:
				MeshDX11(ID3D11DeviceContext* a_DeviceContext);
				MeshDX11(const MeshDX11& a_SubMesh);
				MeshDX11(aiMesh& a_Mesh, ID3D11DeviceContext* a_DeviceContext);
				virtual ~MeshDX11();

				virtual void PreRender();
				virtual void PostRender();

			private:
				virtual void ConnectWithShader(Shader* a_Shader);
				ID3D11DeviceContext* m_DeviceContext;
				Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
				Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;
			};
		}
	}
}

#endif
#ifndef LUX_SUB_MESH_DX11_H
#define LUX_SUB_MESH_DX11_H

namespace Lux
{
	namespace Core
	{
		namespace Internal
		{
			class SubMeshDX11 : public SubMesh
			{
			public:
				SubMeshDX11(ID3D11DeviceContext* a_DeviceContext);
				SubMeshDX11(const SubMeshDX11& a_SubMesh);
				SubMeshDX11(aiMesh& a_Mesh, ID3D11DeviceContext* a_DeviceContext);
				virtual ~SubMeshDX11();

				virtual void PreRender();
				virtual void PostRender();

			private:
				ID3D11DeviceContext* m_DeviceContext;
				Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
				Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;
			};
		}
	}
}

#endif
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
				SubMeshDX11();
				SubMeshDX11(const SubMeshDX11& a_SubMesh);
				SubMeshDX11(aiMesh& a_Mesh);
				virtual ~SubMeshDX11();

				virtual void PreRender() = 0;
				virtual void PostRender() = 0;

			};
		}
	}
}

#endif
#ifndef LUX_MESH_RENDERER_H
#define LUX_MESH_RENDERER_H
#include "LuxComponent.h"
#include "LuxObserverPtr.h"

namespace Lux
{
	namespace Core
	{
		class SubMesh;
	}

	namespace Graphics
	{
		class MeshRenderer : public Core::Component
		{
		public:
			MeshRenderer();
			MeshRenderer(const MeshRenderer& a_Other);
			~MeshRenderer();

			inline void SetMesh(Core::ObserverPtr<Core::SubMesh>& a_Mesh) { m_Mesh.reset(a_Mesh.get()); }
			inline Core::ObserverPtr<Core::SubMesh>& GetMesh() { return m_Mesh; }
		protected:
			Core::ObserverPtr<Core::SubMesh> m_Mesh;
			void Reset();
			friend class Core::ComponentFactory;
		};
	}
}

#endif
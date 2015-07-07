#ifndef LUX_MESH_RENDERER_H
#define LUX_MESH_RENDERER_H
#include "LuxComponent.h"
#include "LuxObserverPtr.h"
#include "LuxMesh.h"
namespace Lux
{
	namespace Core
	{
		class Mesh;
	}

	namespace Graphics
	{
		class MeshRenderer : public Core::Component
		{
		public:
			MeshRenderer();
			MeshRenderer(const MeshRenderer& a_Other);
			~MeshRenderer();

			inline void SetMesh(Core::ObserverPtr<Core::Mesh>& a_Mesh) { m_Mesh.reset(a_Mesh.get()); }
			inline Core::ObserverPtr<Core::Mesh>& GetMesh() { return m_Mesh; }
		protected:
			Core::ObserverPtr<Core::Mesh> m_Mesh;
			void Reset();
			friend class Core::ComponentFactory;
		};
	}
}

#endif
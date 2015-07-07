#ifndef LUX_CONVEX_MESH_COLLIDER_H
#define LUX_CONVEX_MESH_COLLIDER_H
#include "LuxCollider.h"
#include "LuxObserverPtr.h"
#include "LuxModel.h"
namespace Lux
{
	namespace Physics
	{
		class ConvexMeshCollider : public Collider
		{
		public:
			ConvexMeshCollider();
			ConvexMeshCollider(const ConvexMeshCollider& a_Other);
			virtual ~ConvexMeshCollider();

			inline void SetMesh(Core::ObserverPtr<Core::Mesh>& a_Mesh) { m_Mesh.reset(a_Mesh.get()); }
			inline Core::ObserverPtr<Core::Mesh>& GetMesh() { return m_Mesh; }

		private:
			Core::ObserverPtr<Core::Mesh> m_Mesh;

		protected:
			virtual void Reset();
			friend class Core::ComponentFactory;
			friend class PhysicsSystem;
		};
	}
}

#endif
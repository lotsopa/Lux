#ifndef LUX_CONVEX_MESH_COLLIDER_H
#define LUX_CONVEX_MESH_COLLIDER_H
#include "LuxCollider.h"
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

		private:
			
		protected:
			virtual void Reset();
			friend class Core::ComponentFactory;
			friend class PhysicsSystem;
		};
	}
}

#endif
#ifndef LUX_MESH_RENDERER_H
#define LUX_MESH_RENDERER_H
#include "LuxComponent.h"

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
			MeshRenderer(Core::Mesh* a_Mesh);
			~MeshRenderer();

			void SetMesh(Core::Mesh* a_Mesh);
			inline Core::Mesh* GetMesh() { return m_Mesh; }
		protected:
			Core::Mesh* m_Mesh;
			void Reset();
			friend class Core::ComponentFactory;
		};
	}
}

#endif
#ifndef LUX_MESH_RENDERER_H
#define LUX_MESH_RENDERER_H
#include "LuxComponent.h"

namespace Lux
{
	class Mesh;
	class MeshRenderer : public Component
	{
	public:
		MeshRenderer();
		MeshRenderer(Mesh* a_Mesh);
		~MeshRenderer();

		void SetMesh(Mesh* a_Mesh);
	protected:
		Mesh* m_Mesh;
		void Reset();
		friend class ComponentFactory;
	};
}

#endif
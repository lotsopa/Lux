#ifndef LUX_MESH_H
#define LUX_MESH_H
#include "LuxAABB.h"
#include "LuxObserverPtr.h"
#include "LuxTexture.h"
#include "LuxTexture2D.h"
#include "LuxShader.h"
#include "LuxMaterial.h"
#include "LuxSubMesh.h"

namespace Lux
{
	namespace Core
	{
		class SubMesh;
		class MeshAnimation;
		class Shader;
		/*
		Mesh class.
		A mesh can be composed of one or more sub meshes.
		*/
		class Mesh
		{
		public:
			Mesh();
			virtual ~Mesh();

			virtual void AddSubMesh(SubMesh* a_Mesh) = 0;
			virtual void AddAnimation(MeshAnimation* a_Anim) = 0;
			virtual const unsigned int GetNumSubMeshes() = 0;
			virtual ObserverPtr<SubMesh> GetSubMesh(const unsigned int a_Index) = 0;

			inline AABB& GetAABB() { return m_AABB; }

			// Sets the material properties for all the sub meshes
			virtual void SetMaterialProperties(Core::ObserverPtr<Material>& a_Mat) = 0;

			// Sets the diffuse texture for all the sub meshes
			virtual void SetTexture(TextureIndex a_Idx, Core::ObserverPtr<Texture2D>& a_Tex) = 0;

			// Sets the shader for all the sub meshes
			virtual void SetShader(ObserverPtr<Shader>& a_Shader) = 0;

		protected:
			AABB m_AABB;
		};
	}
}

#endif
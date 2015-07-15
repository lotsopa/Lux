#ifndef LUX_MODEL_H
#define LUX_MODEL_H
#include "LuxAABB.h"
#include "LuxObserverPtr.h"
#include "LuxTexture.h"
#include "LuxTexture2D.h"
#include "LuxShader.h"
#include "LuxMaterial.h"
#include "LuxModel.h"

namespace Lux
{
	namespace Core
	{
		class Mesh;
		class MeshAnimation;
		class Shader;
		/*
		Model class.
		A model is composed of one or more meshes.
		*/
		class Model
		{
		public:
			Model();
			virtual ~Model();

			virtual void AddMesh(Mesh* a_Mesh) = 0;
			virtual void AddAnimation(MeshAnimation* a_Anim) = 0;
			virtual const unsigned int GetNumMeshes() = 0;
			virtual ObserverPtr<Mesh> GetMesh(const unsigned int a_Index) = 0;

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
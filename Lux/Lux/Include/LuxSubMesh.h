#ifndef LUX_SUB_MESH_H
#define LUX_SUB_MESH_H
#include "LuxObserverPtr.h"
#include "LuxTexture.h"
#include "LuxTexture2D.h"
#include "LuxShader.h"
#include "LuxMaterial.h"

namespace Lux
{
	namespace Core
	{
		class Vertex;
		class SubMesh
		{
		public:
			SubMesh();
			SubMesh(const SubMesh& a_SubMesh);
			SubMesh(aiMesh& a_Mesh);
			virtual ~SubMesh();

			struct VertexBoneWeight
			{
				VertexBoneWeight();;

				VertexBoneWeight(const unsigned int a_Idx, const float a_Weight);;

				~VertexBoneWeight();;

				// Which vertex is influenced by the bone
				unsigned int m_VertexId;

				// The weight, which is expressed in a normalized range between 0 and 1
				float m_Weight;
			};

			struct Bone
			{
				Bone();;

				Bone(const Bone& a_Bone);

				Bone(aiBone& a_Bone);

				~Bone();;

				String m_Name;
				unsigned int m_NumBoneWeights;
				VertexBoneWeight* m_BoneWeights;
				// Matrix that transforms from mesh space to bone space in bind pose
				mat4x4 m_OffsetMatrix;
			};

			virtual void PreRender() = 0;
			virtual void PostRender() = 0;
			inline Vertex* GetVertices() { return m_Vertices; }
			inline unsigned int* GetIndices() { return m_Indices; }
			inline unsigned int GetNumVertices() { return m_NumVertices; }
			inline unsigned int GetNumIndices() { return m_NumIndices; }

			inline ObserverPtr<Material>& GetMaterialProperties() { return m_MaterialProperties; }
			inline void SetMaterialProperties(Core::ObserverPtr<Material>& a_Mat) { m_MaterialProperties.reset(a_Mat.get()); }
			inline void SetDiffuseTexture(Core::ObserverPtr<Texture2D>& a_Tex) { m_DiffuseTexture.reset(a_Tex.get()); }
			inline Core::ObserverPtr<Texture2D>& GetDiffuseTexture() { return m_DiffuseTexture; }
			inline Core::ObserverPtr<Shader>& GetShader() { return m_Shader; }
			inline void SetShader(ObserverPtr<Shader>& a_Shader) { m_Shader.reset(a_Shader.get()); }

		protected:
			unsigned int m_NumVertices;
			unsigned int m_NumIndices;
			unsigned int m_NumBones;
			unsigned int m_NumComponentsPerTexCoordSet[AI_MAX_NUMBER_OF_TEXTURECOORDS];

			Vertex* m_Vertices;
			unsigned int* m_Indices;
			vec3* m_TextureCoordSets[AI_MAX_NUMBER_OF_TEXTURECOORDS];
			vec4* m_VertexColorSets[AI_MAX_NUMBER_OF_COLOR_SETS];
			Bone** m_Bones;

			ObserverPtr<Material> m_MaterialProperties;
			ObserverPtr<Texture2D> m_DiffuseTexture;
			ObserverPtr<Shader> m_Shader;

			void SafeDeleteAttributes();

		};
	}
}

#endif
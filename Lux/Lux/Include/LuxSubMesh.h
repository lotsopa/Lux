#ifndef LUX_SUB_MESH_H
#define LUX_SUB_MESH_H

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
				VertexBoneWeight() : m_VertexId(0), m_Weight(0.0f)
				{

				};

				VertexBoneWeight(const unsigned int a_Idx, const float a_Weight) : m_VertexId(a_Idx), m_Weight(a_Weight)
				{

				};

				~VertexBoneWeight()
				{

				};

				// Which vertex is influenced by the bone
				unsigned int m_VertexId;

				// The weight, which is expressed in a normalized range between 0 and 1
				float m_Weight;
			};

			struct Bone
			{
				Bone() : m_NumBoneWeights(0), m_Name(), m_BoneWeights(nullptr)
				{

				};

				Bone(const Bone& a_Bone)
				{
					m_NumBoneWeights = a_Bone.m_NumBoneWeights;
					m_Name = a_Bone.m_Name;
					m_BoneWeights = new VertexBoneWeight[m_NumBoneWeights];
					m_OffsetMatrix = a_Bone.m_OffsetMatrix;

					for (unsigned int i = 0; i < m_NumBoneWeights; i++)
					{
						m_BoneWeights[i].m_VertexId = a_Bone.m_BoneWeights[i].m_VertexId;
						m_BoneWeights[i].m_Weight = a_Bone.m_BoneWeights[i].m_Weight;
					}
				}

				Bone(aiBone& a_Bone)
				{
					m_NumBoneWeights = a_Bone.mNumWeights;
					m_Name = a_Bone.mName.C_Str();
					m_BoneWeights = new VertexBoneWeight[m_NumBoneWeights];
					m_OffsetMatrix = Utility::ConvertMatrixAssimp(a_Bone.mOffsetMatrix);

					for (unsigned int i = 0; i < m_NumBoneWeights; i++)
					{
						m_BoneWeights[i].m_VertexId = a_Bone.mWeights[i].mVertexId;
						m_BoneWeights[i].m_Weight = a_Bone.mWeights[i].mWeight;
					}
				}

				~Bone()
				{
					Utility::SafeArrayDelete(m_BoneWeights);
				};

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

			void SafeDeleteAttributes();

		};
	}
}

#endif
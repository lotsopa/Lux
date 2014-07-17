#ifndef LUX_SUB_MESH_H
#define LUX_SUB_MESH_H

namespace Lux
{
	class Material;
	class SubMesh
	{
	public:
		SubMesh();
		SubMesh(aiMesh& a_Mesh);
		~SubMesh();

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

			Bone(aiBone& a_Bone)
			{
				m_NumBoneWeights = a_Bone.mNumWeights;
				m_Name = a_Bone.mName.C_Str();
				m_BoneWeights = new VertexBoneWeight[m_NumBoneWeights];
				m_OffsetMatrix = ConvertMatrixAssimpToGLM(a_Bone.mOffsetMatrix);

				for (unsigned int i = 0; i < m_NumBoneWeights; i++)
				{
					m_BoneWeights[i].m_VertexId = a_Bone.mWeights[i].mVertexId;
					m_BoneWeights[i].m_Weight = a_Bone.mWeights[i].mWeight;
				}
			}

			~Bone()
			{
				delete[] m_BoneWeights;
			};

			String m_Name;
			unsigned int m_NumBoneWeights;
			VertexBoneWeight* m_BoneWeights;
			// Matrix that transforms from mesh space to bone space in bind pose
			mat4x4 m_OffsetMatrix;
		};

		Material* GetMaterial();
		void SetMaterial(Material* a_Mat);
		
	private:
		unsigned int m_NumVertices;
		unsigned int m_NumIndices;
		unsigned int m_NumBones;
		Material* m_Material;
		unsigned int m_NumComponentsPerTexCoordSet[AI_MAX_NUMBER_OF_TEXTURECOORDS];

		vec3* m_Vertices;
		unsigned int* m_Indices;
		vec3* m_TextureCoordSets[AI_MAX_NUMBER_OF_TEXTURECOORDS];
		vec3* m_VertexTangents;
		vec3* m_VertexNormals;
		vec3* m_VertexBitangents;
		vec4* m_VertexColorSets[AI_MAX_NUMBER_OF_COLOR_SETS];
		Bone** m_Bones;

		void SafeDeleteAttributes();
		
	};
}

#endif
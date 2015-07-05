#include "LuxPCH.h"
#include "LuxMaterial.h"
#include "LuxVertex.h"
#include "LuxSubMesh.h"

Lux::Core::SubMesh::SubMesh() :
m_Bones(nullptr),
m_Indices(nullptr),
m_NumBones(0),
m_NumIndices(0),
m_NumVertices(0),
m_Vertices(nullptr)
{
	for (unsigned int i = 0; i < AI_MAX_NUMBER_OF_COLOR_SETS; i++)
	{
		m_VertexColorSets[i] = nullptr;
	}

	for (unsigned int i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; i++)
	{
		m_TextureCoordSets[i] = nullptr;
		m_NumComponentsPerTexCoordSet[i] = 0;
	}
}

Lux::Core::SubMesh::SubMesh(aiMesh& a_Mesh) :
SubMesh()
{
	// Fill vertices
	m_NumVertices = a_Mesh.mNumVertices;
	m_Vertices = new Vertex[m_NumVertices];
	for (unsigned int i = 0; i < m_NumVertices; i++)
	{
		m_Vertices[i].m_Position = vec3(a_Mesh.mVertices[i].x, a_Mesh.mVertices[i].y, a_Mesh.mVertices[i].z);
	}

	// Fill tangents and bi tangents
	if (a_Mesh.HasTangentsAndBitangents())
	{
		for (unsigned int i = 0; i < m_NumVertices; i++)
		{
			m_Vertices[i].m_Tangent = vec3(a_Mesh.mTangents[i].x, a_Mesh.mTangents[i].y, a_Mesh.mTangents[i].z);
		}

		for (unsigned int i = 0; i < m_NumVertices; i++)
		{
			m_Vertices[i].m_Bitangent = vec3(a_Mesh.mBitangents[i].x, a_Mesh.mBitangents[i].y, a_Mesh.mBitangents[i].z);
		}
	}

	// Fill normals
	if (a_Mesh.HasNormals())
	{
		for (unsigned int i = 0; i < m_NumVertices; i++)
		{
			m_Vertices[i].m_Normal = vec3(a_Mesh.mNormals[i].x, a_Mesh.mNormals[i].y, a_Mesh.mNormals[i].z);
		}
	}

	// Fill color data
	for (unsigned int i = 0; i < AI_MAX_NUMBER_OF_COLOR_SETS; i++)
	{
		if (a_Mesh.HasVertexColors(i))
		{
			m_VertexColorSets[i] = new vec4[m_NumVertices];
			for (unsigned int j = 0; j < m_NumVertices; j++) // RGBA
			{
				m_VertexColorSets[i][j] = vec4(a_Mesh.mColors[i][j].r, a_Mesh.mColors[i][j].g, a_Mesh.mColors[i][j].b, a_Mesh.mColors[i][j].a);

				// Only 1 set supported in the vertices for now
				if (i == 0)
				{
					m_Vertices[i].m_Color = m_VertexColorSets[i][j];
				}
			}
		}
	}

	// Fill texture coordinate data
	for (unsigned int i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; i++)
	{
		if (a_Mesh.HasTextureCoords(i))
		{
			m_TextureCoordSets[i] = new vec3[m_NumVertices];
			for (unsigned int j = 0; j < m_NumVertices; j++)
			{
				m_TextureCoordSets[i][j] = vec3(a_Mesh.mTextureCoords[i][j].x, a_Mesh.mTextureCoords[i][j].y, a_Mesh.mTextureCoords[i][j].z);
				// Only 1 set supported in the vertices for now
				if (i == 0)
				{
					m_Vertices[j].m_TexCoords = vec2(m_TextureCoordSets[i][j].x, m_TextureCoordSets[i][j].y);
				}
			}
		}
		m_NumComponentsPerTexCoordSet[i] = a_Mesh.mNumUVComponents[i];
	}

	// Filling indices
	std::vector<unsigned int> indexVec; // Using temporary vector for convenience
	for (unsigned int i = 0; i < a_Mesh.mNumFaces; i++)
	{
		for (unsigned int j = 0; j < a_Mesh.mFaces[i].mNumIndices; j++)
		{
			indexVec.push_back(a_Mesh.mFaces[i].mIndices[j]);
		}
	}
	m_NumIndices = indexVec.size();

	if (m_NumIndices)
	{
		m_Indices = new unsigned int[m_NumIndices];

		for (unsigned int i = 0; i < m_NumIndices; i++)
		{
			m_Indices[i] = indexVec[i];
		}
	}

	// Fill bones
	m_NumBones = a_Mesh.mNumBones;
	if (a_Mesh.HasBones())
	{
		m_Bones = new Bone*[m_NumBones];
		for (unsigned int i = 0; i < m_NumBones; i++)
		{
			m_Bones[i] = new Bone(*a_Mesh.mBones[i]);
		}
	}
}

Lux::Core::SubMesh::SubMesh(const SubMesh& a_SubMesh)
{
	// Fill vertices
	m_NumVertices = a_SubMesh.m_NumVertices;
	m_Vertices = new Vertex[m_NumVertices];
	for (unsigned int i = 0; i < m_NumVertices; i++)
	{
		m_Vertices[i] = a_SubMesh.m_Vertices[i];
	}

	// Fill color data
	for (unsigned int i = 0; i < AI_MAX_NUMBER_OF_COLOR_SETS; i++)
	{
		if (a_SubMesh.m_VertexColorSets[i] != nullptr)
		{
			m_VertexColorSets[i] = new vec4[m_NumVertices];
			for (unsigned int j = 0; j < m_NumVertices; j++) // RGBA
			{
				m_VertexColorSets[i][j] = vec4(*a_SubMesh.m_VertexColorSets[i]);
			}
		}
	}
	// Fill texture coordinate data
	for (unsigned int i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; i++)
	{
		if (a_SubMesh.m_TextureCoordSets[i] != nullptr)
		{
			m_TextureCoordSets[i] = new vec3[m_NumVertices];
			for (unsigned int j = 0; j < m_NumVertices; j++)
			{
				m_TextureCoordSets[i][j] = vec3(*a_SubMesh.m_TextureCoordSets[i]);
			}
		}
		m_NumComponentsPerTexCoordSet[i] = a_SubMesh.m_NumComponentsPerTexCoordSet[i];
	}

	// Filling indices
	m_NumIndices = a_SubMesh.m_NumIndices;

	if (m_NumIndices)
	{
		m_Indices = new unsigned int[m_NumIndices];

		for (unsigned int i = 0; i < m_NumIndices; i++)
		{
			m_Indices[i] = a_SubMesh.m_Indices[i];
		}
	}

	// Fill bones
	m_NumBones = a_SubMesh.m_NumBones;
	if (m_NumBones)
	{
		m_Bones = new Bone*[m_NumBones];
		for (unsigned int i = 0; i < m_NumBones; i++)
		{
			m_Bones[i] = new Bone(*a_SubMesh.m_Bones[i]);
		}
	}
}

Lux::Core::SubMesh::~SubMesh()
{
	//SafeDeleteAttributes();
}

void Lux::Core::SubMesh::SafeDeleteAttributes()
{
	Utility::SafeArrayDelete(m_Vertices);
	Utility::SafeArrayDelete(m_Indices);
	// Materials are managed by the resource manager, so don't destroy them here

	if (m_NumBones)
	{
		for (unsigned int i = 0; i < m_NumBones; i++)
		{
			Utility::SafePtrDelete(m_Bones[i]);
		}
		Utility::SafeArrayDelete(m_Bones);
	}

	for (int i = 0; i < AI_MAX_NUMBER_OF_COLOR_SETS; i++)
	{
		Utility::SafeArrayDelete(m_VertexColorSets[i]);
	}

	for (int i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; i++)
	{
		Utility::SafeArrayDelete(m_TextureCoordSets[i]);
	}
}

Lux::Core::SubMesh::Bone::~Bone()
{
	Utility::SafeArrayDelete(m_BoneWeights);
}

Lux::Core::SubMesh::Bone::Bone(aiBone& a_Bone)
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

Lux::Core::SubMesh::Bone::Bone(const Bone& a_Bone)
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

Lux::Core::SubMesh::Bone::Bone() : m_NumBoneWeights(0), m_Name(), m_BoneWeights(nullptr)
{

}

Lux::Core::SubMesh::VertexBoneWeight::~VertexBoneWeight()
{

}

Lux::Core::SubMesh::VertexBoneWeight::VertexBoneWeight(const unsigned int a_Idx, const float a_Weight) : m_VertexId(a_Idx), m_Weight(a_Weight)
{

}

Lux::Core::SubMesh::VertexBoneWeight::VertexBoneWeight() : m_VertexId(0), m_Weight(0.0f)
{

}

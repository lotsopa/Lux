#include "LuxPCH.h"
#include "LuxMaterial.h"
#include "LuxSubMesh.h"

Lux::SubMesh::SubMesh() :
m_Bones(nullptr),
m_Indices(nullptr),
m_VertexNormals(nullptr),
m_Material(nullptr),
m_NumBones(0),
m_NumIndices(0),
m_NumVertices(0),
m_VertexTangents(nullptr),
m_Vertices(nullptr),
m_VertexBitangents(nullptr)
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

Lux::SubMesh::SubMesh(aiMesh& a_Mesh) : 
SubMesh()
{
	// Fill vertices
	m_NumVertices = a_Mesh.mNumVertices;
	m_Vertices = new vec3[m_NumVertices];
	for (unsigned int i = 0; i < m_NumVertices; i++)
	{
		m_Vertices[i] = vec3(a_Mesh.mVertices[i].x, a_Mesh.mVertices[i].y, a_Mesh.mVertices[i].z);
	}

	// Fill tangents and bi tangents
	if (a_Mesh.HasTangentsAndBitangents())
	{
		m_VertexTangents = new vec3[m_NumVertices];
		for (unsigned int i = 0; i < m_NumVertices; i++)
		{
			m_VertexTangents[i] = vec3(a_Mesh.mTangents[i].x, a_Mesh.mTangents[i].y, a_Mesh.mTangents[i].z);
		}

		m_VertexBitangents = new vec3[m_NumVertices];
		for (unsigned int i = 0; i < m_NumVertices; i++)
		{
			m_VertexBitangents[i] = vec3(a_Mesh.mBitangents[i].x, a_Mesh.mBitangents[i].y, a_Mesh.mBitangents[i].z);
		}
	}

	// Fill normals
	if (a_Mesh.HasNormals())
	{
		m_VertexNormals = new vec3[m_NumVertices];
		for (unsigned int i = 0; i < m_NumVertices; i++)
		{
			m_VertexNormals[i] = vec3(a_Mesh.mNormals[i].x, a_Mesh.mNormals[i].y, a_Mesh.mNormals[i].z);
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

Lux::SubMesh::SubMesh(const SubMesh& a_SubMesh)
{
	// Fill vertices
	m_NumVertices = a_SubMesh.m_NumVertices;
	m_Vertices = new vec3[m_NumVertices];
	for (unsigned int i = 0; i < m_NumVertices; i++)
	{
		m_Vertices[i] = a_SubMesh.m_Vertices[i];
	}

	// Fill tangents and bi tangents
	if (a_SubMesh.m_VertexTangents != nullptr && a_SubMesh.m_VertexBitangents != nullptr)
	{
		m_VertexTangents = new vec3[m_NumVertices];
		for (unsigned int i = 0; i < m_NumVertices; i++)
		{
			m_VertexTangents[i] = a_SubMesh.m_VertexTangents[i];
		}

		m_VertexBitangents = new vec3[m_NumVertices];
		for (unsigned int i = 0; i < m_NumVertices; i++)
		{
			m_VertexBitangents[i] = a_SubMesh.m_VertexBitangents[i];
		}
	}

	// Fill normals
	if (a_SubMesh.m_VertexNormals != nullptr)
	{
		m_VertexNormals = new vec3[m_NumVertices];
		for (unsigned int i = 0; i < m_NumVertices; i++)
		{
			m_VertexNormals[i] = a_SubMesh.m_VertexNormals[i];
		}
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

Lux::SubMesh::~SubMesh()
{
	SafeDeleteAttributes();
}

void Lux::SubMesh::SafeDeleteAttributes()
{
	SafeArrayDelete(m_Vertices);
	SafeArrayDelete(m_Indices);
	SafeArrayDelete(m_VertexNormals);
	SafeArrayDelete(m_VertexTangents);
	SafeArrayDelete(m_VertexBitangents);
	// Materials are managed by the resource manager, so don't destroy them here

	if (m_NumBones)
	{
		for (unsigned int i = 0; i < m_NumBones; i++)
		{
			SafePtrDelete(m_Bones[i]);
		}
		SafeArrayDelete(m_Bones);
	}

	for (int i = 0; i < AI_MAX_NUMBER_OF_COLOR_SETS; i++)
	{
		SafeArrayDelete(m_VertexColorSets[i]);
	}

	for (int i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; i++)
	{
		SafeArrayDelete(m_TextureCoordSets[i]);
	}
}

Lux::Material* Lux::SubMesh::GetMaterial()
{
	return m_Material;
}

void Lux::SubMesh::SetMaterial(Material* a_Mat)
{
	LuxAssert(a_Mat);

	if (a_Mat == nullptr)
	{
		LUX_LOG(logWARNING) << "NULL passed to SetMaterial() function. Nothing done.";
	}

	m_Material = a_Mat;
}

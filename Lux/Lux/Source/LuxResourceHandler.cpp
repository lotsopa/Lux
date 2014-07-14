#include "LuxPCH.h"
#include "LuxKey.h"
#include "LuxMaterial.h"
#include "LuxMesh.h"
#include "LuxEntity.h"
#include "LuxResourceHandler.h"
#include "LuxFileHandler.h"

Lux::ResourceHandler& Lux::ResourceHandler::GetInstance()
{
	static ResourceHandler instance;
	return instance;
}

Lux::ResourceHandler::~ResourceHandler()
{
	EntityMap::iterator it;

	for (it = m_EntityMap.begin(); it != m_EntityMap.end(); ++it)
	{
		delete it->second;
	}
	m_EntityMap.clear();

	MaterialMap::iterator it2;

	for (it2 = m_MaterialMap.begin(); it2 != m_MaterialMap.end(); ++it2)
	{
		delete it2->second;
	}
	m_MaterialMap.clear();
}

Lux::ResourceHandler::ResourceHandler()
{
	LUX_LOG(logINFO) << "Resource Handler created successfully.";
}

Lux::Entity* Lux::ResourceHandler::CreateEntityFromFile(const String a_File, const String a_EntityName, unsigned int a_PostProcessFlags)
{
	FileInfo* file = FileHandler::GetInstance().LoadFileInMemory(a_File);

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFileFromMemory(file->m_RawData, file->m_DataLength, a_PostProcessFlags);
	if (scene == nullptr)
	{
		String errStr("Failed to read file. ");
		errStr.append(importer.GetErrorString());
		ThrowError(errStr);
	}

	if (scene->mNumMeshes < 1)
	{
		String errStr("The file " + a_File + " does not contain any meshes.");
		ThrowError(errStr);
	}

	// Materials
	for (unsigned int i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* impMat = scene->mMaterials[i];
		aiString str;
		impMat->Get(AI_MATKEY_NAME, str);
		String matName = str.C_Str();
		Material* myMat = new Material(*impMat);
		AddMaterialToMap(matName, myMat);
	}

	// Creating an entity and adding meshes
	Entity* retEntity = new Entity(scene->mNumMeshes);
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		aiMaterial* impMat = scene->mMaterials[scene->mMeshes[i]->mMaterialIndex];
		aiString str;
		impMat->Get(AI_MATKEY_NAME, str);
		String matName = str.C_Str();
		Material* meshMat = GetMaterial(matName);
		Mesh* mesh = new Mesh(*scene->mMeshes[i]);
		mesh->SetMaterial(meshMat);
		retEntity->AddMesh(mesh);
	}

	AddEntityToMap(a_EntityName, retEntity);
	return retEntity;
}

Lux::Entity* Lux::ResourceHandler::CreateEntityFromMemory(FileInfo* a_Info, const String a_EntityName, unsigned int a_PostProcessFlags)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFileFromMemory(a_Info->m_RawData, a_Info->m_DataLength, a_PostProcessFlags);
	if (scene == nullptr)
	{
		String errStr("Failed to read file. ");
		errStr.append(importer.GetErrorString());
		ThrowError(errStr);
	}

	if (scene->mNumMeshes < 1)
	{
		return nullptr;
	}

	Entity* retEntity = new Entity(scene->mNumMeshes);
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		Mesh* mesh = new Mesh(*scene->mMeshes[i]);
		retEntity->AddMesh(mesh);
	}

	AddEntityToMap(a_EntityName, retEntity);
	return retEntity;
}

#if LUX_THREAD_SAFE == TRUE
void Lux::ResourceHandler::AddEntityToMap(const String& a_Str, Entity* a_Ent)
{
	std::unique_lock<std::mutex> lock(m_EntityMapMutex); // Upon construction of the lock the mutex will be immediately locked
	m_EntityMap.insert(std::make_pair(Key(a_Str), a_Ent));
}
Lux::Entity* Lux::ResourceHandler::GetEntity(const String a_Name)
{
	std::unique_lock<std::mutex> lock(m_EntityMapMutex); // Upon construction of the lock the mutex will be immediately locked
	return m_EntityMap.at(Key(a_Name));
}

void Lux::ResourceHandler::AddMaterialToMap(const String& a_Str, Material* a_Mat)
{
	std::unique_lock<std::mutex> lock(m_MaterialMapMutex); // Upon construction of the lock the mutex will be immediately locked
	m_MaterialMap.insert(std::make_pair(Key(a_Str), a_Mat));
}

Lux::Material* Lux::ResourceHandler::GetMaterial(const String a_Name)
{
	std::unique_lock<std::mutex> lock(m_MaterialMapMutex); // Upon construction of the lock the mutex will be immediately locked
	return m_MaterialMap.at(Key(a_Name));
}

bool Lux::ResourceHandler::MaterialExists(const String a_Name)
{
	Key k(a_Name);
	std::unique_lock<std::mutex> lock(m_MaterialMapMutex); // Upon construction of the lock the mutex will be immediately locked
	unsigned int count = m_MaterialMap.count(k);

	if (count > 0)
	{
		return true;
	}

	return false;
}

bool Lux::ResourceHandler::EntityExists(const String a_Name)
{
	Key k(a_Name);
	std::unique_lock<std::mutex> lock(m_EntityMapMutex); // Upon construction of the lock the mutex will be immediately locked
	unsigned int count = m_EntityMap.count(k);

	if (count > 0)
	{
		return true;
	}

	return false;
}

#else
void Lux::ResourceHandler::AddEntityToMap(const String& a_Str, Entity* a_Ent)
{
	m_EntityMap.insert(std::make_pair(Key(a_Str), a_Ent));
}

Lux::Entity* Lux::ResourceHandler::GetEntity(const String a_Name)
{
	return m_EntityMap.at(Key(a_Name));
}

void Lux::ResourceHandler::AddMaterialToMap(const String& a_Str, Material* a_Mat)
{
	m_MaterialMap.insert(std::make_pair(Key(a_Str), a_Mat));
}

Lux::Material* Lux::ResourceHandler::GetMaterial(const String a_Name)
{
	return m_MaterialMap.at(Key(a_Name));
}

bool Lux::ResourceHandler::MaterialExists(const String a_Name)
{
	Key k(a_Name);
	unsigned int count = m_MaterialMap.count(k);

	if (count > 0)
	{
		return true;
	}

	return false;
}

bool Lux::ResourceHandler::EntityExists(const String a_Name)
{
	Key k(a_Name);
	unsigned int count = m_EntityMap.count(k);

	if (count > 0)
	{
		return true;
	}

	return false;
}

#endif // LUX_THREAD_SAFE

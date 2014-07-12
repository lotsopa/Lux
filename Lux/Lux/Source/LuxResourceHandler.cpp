#include "LuxPCH.h"
#include "LuxKey.h"
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

}

Lux::ResourceHandler::ResourceHandler()
{

}

Lux::Entity* Lux::ResourceHandler::CreateEntityFromFile(const String a_File, const String a_MeshName, unsigned int a_PostProcessFlags)
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
		return nullptr;
	}
	
	// TODO

	return nullptr;
}

Lux::Entity* Lux::ResourceHandler::CreateEntityFromMemory(FileInfo* a_Info, const String a_MeshName, unsigned int a_PostProcessFlags)
{
	//TODO
	return nullptr;
}

#include "LuxPCH.h"
#include "LuxKey.h"
#include "LuxMaterial.h"
#include "LuxMesh.h"
#include "LuxEntity.h"
#include "LuxEntityAnimation.h"
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

	TextureMap::iterator it3;

	for (it3 = m_TextureMap.begin(); it3 != m_TextureMap.end(); ++it3)
	{
		glDeleteTextures(1, &it3->second);
	}
	m_TextureMap.clear();
}

Lux::ResourceHandler::ResourceHandler()
{
	LUX_LOG(logINFO) << "Resource Handler created successfully.";
}

Lux::Entity* Lux::ResourceHandler::CreateEntityFromFile(const String& a_File, const String& a_EntityName, unsigned int a_PostProcessFlags)
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

		// Load all textures from all the types. 
		// If there are no textures from the specified type the function will just do nothing.
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_DIFFUSE);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_SPECULAR);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_AMBIENT);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_EMISSIVE);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_NORMALS);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_HEIGHT);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_LIGHTMAP);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_REFLECTION);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_DISPLACEMENT);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_OPACITY);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_SHININESS);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_UNKNOWN);
	}

	// Creating an entity and adding meshes
	Entity* retEntity = new Entity(scene->mNumMeshes, scene->mNumAnimations);
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

	// Animation data (if it exists)
	if (scene->HasAnimations())
	{
		for (unsigned int i = 0; i < scene->mNumAnimations; i++)
		{
			EntityAnimation* animData = new EntityAnimation(*scene->mAnimations[i]);
			retEntity->AddAnimation(animData);
		}
	}

	AddEntityToMap(a_EntityName, retEntity);
	return retEntity;
}

Lux::Entity* Lux::ResourceHandler::CreateEntityFromMemory(FileInfo* a_Info, const String& a_EntityName, unsigned int a_PostProcessFlags)
{
	if (a_Info == nullptr)
	{
		ThrowError("Failed to create entity from Memory. The passed FileInfo pointer is NULL.");
	}

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
		String errStr("The file does not contain any meshes.");
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

		// Load all textures from all the types. 
		// If there are no textures from the specified type the function will just do nothing.
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_DIFFUSE);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_SPECULAR);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_AMBIENT);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_EMISSIVE);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_NORMALS);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_HEIGHT);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_LIGHTMAP);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_REFLECTION);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_DISPLACEMENT);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_OPACITY);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_SHININESS);
		LoadAllTexturesOfTypeFromMaterial(impMat, aiTextureType_UNKNOWN);
	}

	// Creating an entity and adding meshes
	Entity* retEntity = new Entity(scene->mNumMeshes, scene->mNumAnimations);
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

	// Animation data (if it exists)
	if (scene->HasAnimations())
	{
		for (unsigned int i = 0; i < scene->mNumAnimations; i++)
		{
			EntityAnimation* animData = new EntityAnimation(*scene->mAnimations[i]);
			retEntity->AddAnimation(animData);
		}
	}

	AddEntityToMap(a_EntityName, retEntity);
	return retEntity;
}

void Lux::ResourceHandler::LoadAllTexturesOfTypeFromMaterial(aiMaterial* a_Mat, aiTextureType a_TexType)
{
	aiString texName;
	unsigned int numTextures = a_Mat->GetTextureCount(a_TexType);

	if (!numTextures)
	{
		return;
	}

	for (unsigned int i = 0; i < numTextures; i++)
	{
		aiReturn texFound = a_Mat->GetTexture(a_TexType, i, &texName);
		if (texFound != AI_SUCCESS)
		{
			LUX_LOG(logWARNING) << "Could not find texture " << texName.C_Str();
			continue;
		}
		CreateTextureFromFile(String(texName.C_Str()), String(texName.C_Str()));
	}
}

const Lux::TextureIndex Lux::ResourceHandler::CreateTextureFromFile(const String& a_File, const String& a_TexName)
{
	FileInfo* file = FileHandler::GetInstance().LoadFileInMemory(a_File);

	FIMEMORY* freeImgMemoryPtr = nullptr;
	freeImgMemoryPtr = FreeImage_OpenMemory((unsigned char*)file->m_RawData, file->m_DataLength);
	FREE_IMAGE_FORMAT imgFormat = FreeImage_GetFileTypeFromMemory(freeImgMemoryPtr, file->m_DataLength);

	if (imgFormat == FIF_UNKNOWN)
	{
		imgFormat = FreeImage_GetFIFFromFilename(a_File.c_str());

		// If it's still unknown, throw
		if (imgFormat == FIF_UNKNOWN)
		{
			String err("The file " + a_File + " Could not be loaded. Unknown format.");
			ThrowError(err);
		}
	}

	//Does the extension support reading?
	if (!FreeImage_FIFSupportsReading(imgFormat))
	{
		String err("The file " + a_File + " Could not be loaded. The file extension does not support reading.");
		ThrowError(err);
	}

	// Load the file in a bitmap
	FIBITMAP* bitmap = FreeImage_LoadFromMemory(imgFormat, freeImgMemoryPtr);
	FIBITMAP* convertedBitmap = FreeImage_ConvertTo32Bits(bitmap);

	if (convertedBitmap == nullptr)
	{
		String err("The file " + a_File + " Could not be loaded. Loading function returned NULL.");
		ThrowError(err);
	}

	unsigned int imgWidth = FreeImage_GetWidth(convertedBitmap);
	unsigned int imgHeight = FreeImage_GetHeight(convertedBitmap);
	unsigned char* bits = FreeImage_GetBits(convertedBitmap);

	// Normally this should never happen, but just to be safe
	if (imgWidth == 0 || imgHeight == 0 || bits == nullptr)
	{
		String err("The file " + a_File + " Could not be created. Failed to retrieve proper data from the loaded file.");
		ThrowError(err);
	}

	// Check if a texture with this name already exists
	if (TextureExists(a_TexName))
	{
		// Delete the old one
		DeleteTexture(a_TexName);
	}

	TextureIndex generatedTexIdx = -1;
	// Generate an openGL ID for the texture
	glGenTextures(1, &generatedTexIdx);

	// Put it in the map
	AddTextureToMap(a_TexName, generatedTexIdx);

	// Bind the texture to set the texture parameters
	if (!BindTexture(a_TexName))
	{
		String err("The file " + a_File + " Could not be created. Failed to bind OpenGL texture.");
		ThrowError(err);
	}

	//store the texture data for OpenGL use (FreeImage loads in BGRA order)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, imgWidth, imgHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, bits);

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	UnbindCurrentTexture();

	//Free FreeImage's copy of the data
	FreeImage_Unload(convertedBitmap);
	return generatedTexIdx;
}


bool Lux::ResourceHandler::BindTexture(const String& a_Name)
{
	bool result = true;
	//if this texture ID mapped, bind it's texture as current
	if (TextureExists(a_Name))
	{
		TextureIndex idx = GetTextureIndex(a_Name);
		glBindTexture(GL_TEXTURE_2D, idx);
	}
	else
	{
		result = false;
	}

	return result;
}


void Lux::ResourceHandler::UnbindCurrentTexture()
{
	glBindTexture(GL_TEXTURE_2D, NULL);
}


#if LUX_THREAD_SAFE == TRUE
void Lux::ResourceHandler::AddEntityToMap(const String& a_Str, Entity* a_Ent)
{
	std::unique_lock<std::mutex> lock(m_EntityMapMutex); // Upon construction of the lock the mutex will be immediately locked
	m_EntityMap.insert(std::make_pair(Key(a_Str), a_Ent));
}
Lux::Entity* Lux::ResourceHandler::GetEntity(const String& a_Name)
{
	std::unique_lock<std::mutex> lock(m_EntityMapMutex); // Upon construction of the lock the mutex will be immediately locked
	return m_EntityMap.at(Key(a_Name));
}

void Lux::ResourceHandler::AddMaterialToMap(const String& a_Str, Material* a_Mat)
{
	std::unique_lock<std::mutex> lock(m_MaterialMapMutex); // Upon construction of the lock the mutex will be immediately locked
	m_MaterialMap.insert(std::make_pair(Key(a_Str), a_Mat));
}

Lux::Material* Lux::ResourceHandler::GetMaterial(const String& a_Name)
{
	std::unique_lock<std::mutex> lock(m_MaterialMapMutex); // Upon construction of the lock the mutex will be immediately locked
	return m_MaterialMap.at(Key(a_Name));
}

bool Lux::ResourceHandler::MaterialExists(const String& a_Name)
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

bool Lux::ResourceHandler::EntityExists(const String& a_Name)
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

bool Lux::ResourceHandler::TextureExists(const String& a_Name)
{
	Key k(a_Name);
	std::unique_lock<std::mutex> lock(m_TextureMapMutex); // Upon construction of the lock the mutex will be immediately locked
	unsigned int count = m_TextureMap.count(k);

	if (count > 0)
	{
		return true;
	}

	return false;
}

Lux::TextureIndex Lux::ResourceHandler::GetTextureIndex(const String& a_Name)
{
	std::unique_lock<std::mutex> lock(m_TextureMapMutex);
	return m_TextureMap.at(Key(a_Name));
}

void Lux::ResourceHandler::AddTextureToMap(const String& a_Str, TextureIndex a_Idx)
{
	std::unique_lock<std::mutex> lock(m_TextureMapMutex);
	m_TextureMap.insert(std::make_pair(Key(a_Str), a_Idx));
}

bool Lux::ResourceHandler::DeleteTexture(const String& a_Name)
{
	if (!TextureExists(a_Name))
	{
		LUX_LOG(logWARNING) << "Could not delete texture with name. " << a_Name << " Texture doesn't exist.";
		return false;
	}
	TextureIndex idx = GetTextureIndex(a_Name);
	glDeleteTextures(1, &idx);

	std::unique_lock<std::mutex> lock(m_TextureMapMutex);
	m_TextureMap.erase(Key(a_Name));

	return true;
}

#else
void Lux::ResourceHandler::AddEntityToMap(const String& a_Str, Entity* a_Ent)
{
	m_EntityMap.insert(std::make_pair(Key(a_Str), a_Ent));
}

Lux::Entity* Lux::ResourceHandler::GetEntity(const String& a_Name)
{
	return m_EntityMap.at(Key(a_Name));
}

void Lux::ResourceHandler::AddMaterialToMap(const String& a_Str, Material* a_Mat)
{
	m_MaterialMap.insert(std::make_pair(Key(a_Str), a_Mat));
}

Lux::Material* Lux::ResourceHandler::GetMaterial(const String& a_Name)
{
	return m_MaterialMap.at(Key(a_Name));
}

bool Lux::ResourceHandler::MaterialExists(const String& a_Name)
{
	Key k(a_Name);
	unsigned int count = m_MaterialMap.count(k);

	if (count > 0)
	{
		return true;
	}

	return false;
}

bool Lux::ResourceHandler::EntityExists(const String& a_Name)
{
	Key k(a_Name);
	unsigned int count = m_EntityMap.count(k);

	if (count > 0)
	{
		return true;
	}

	return false;
}

bool Lux::ResourceHandler::TextureExists(const String& a_Name)
{
	Key k(a_Name);
	unsigned int count = m_TextureMap.count(k);

	if (count > 0)
	{
		return true;
	}

	return false;
}

Lux::TextureIndex Lux::ResourceHandler::GetTextureIndex(const String& a_Name)
{
	return m_TextureMap.at(Key(a_Name));
}

void Lux::ResourceHandler::AddTextureToMap(const String& a_Str, TextureIndex a_Idx)
{
	m_TextureMap.insert(std::make_pair(Key(a_Str), a_Idx));
}

bool Lux::ResourceHandler::DeleteTexture(const String& a_Name)
{
	if (!TextureExists(a_Name))
	{
		LUX_LOG(logWARNING) << "Could not delete texture with name. " << a_Name << " Texture doesn't exist.";
		return false;
	}
	TextureIndex idx = GetTextureIndex(a_Name);
	glDeleteTextures(1, &idx);
	m_TextureMap.erase(Key(a_Name));

	return true;
}
#endif // LUX_THREAD_SAFE

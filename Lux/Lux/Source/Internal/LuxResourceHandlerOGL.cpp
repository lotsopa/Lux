#include "LuxPCH.h"
#include "LuxKey.h"
#include "LuxMaterial.h"
#include "LuxSubMesh.h"
#include "LuxMesh.h"
#include "LuxMeshAnimation.h"
#include "LuxResourceHandler.h"
#include "LuxResourceHandlerOGL.h"
#include "LuxTexture.h"
#include "LuxTexture2D.h"
#include "LuxTexture2DOGL.h"
#include "LuxFileHandler.h"

#ifndef YY_NO_UNISTD_H
#define YY_NO_UNISTD_H
#endif
extern "C"
{
#include "LuxFileScanner.h"
}

Lux::Core::Internal::ResourceHandlerOGL::~ResourceHandlerOGL()
{
	MeshMap::iterator it;

	for (it = m_MeshMap.begin(); it != m_MeshMap.end(); ++it)
	{
		it->second.reset();
	}
	m_MeshMap.clear();

	MaterialMap::iterator it2;

	for (it2 = m_MaterialMap.begin(); it2 != m_MaterialMap.end(); ++it2)
	{
		it2->second.reset();
	}
	m_MaterialMap.clear();

	TextureMap::iterator it3;

	for (it3 = m_TextureMap.begin(); it3 != m_TextureMap.end(); ++it3)
	{
		it3->second.reset();
	}
	m_TextureMap.clear();
}

Lux::Core::Internal::ResourceHandlerOGL::ResourceHandlerOGL()
{
	LUX_LOG(Utility::logINFO) << "Resource Handler created successfully.";
}

Lux::Core::Mesh* Lux::Core::Internal::ResourceHandlerOGL::CreateMeshFromFile(const String& a_File, const String& a_EntityName, unsigned int a_PostProcessFlags)
{
	// Have we already loaded this mesh before? If we have just return it.
	Mesh* loadedMesh = GetLoadedMesh(a_File);
	if (loadedMesh != nullptr)
	{
		return loadedMesh;
	}

	FileInfo* file = FileHandler::GetInstance().LoadFileInMemory(a_File);

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFileFromMemory(file->m_RawData, file->m_DataLength, a_PostProcessFlags);
	if (scene == nullptr)
	{
		String errStr("Failed to read file. ");
		errStr.append(importer.GetErrorString());
		Utility::ThrowError(errStr);
	}

	if (scene->mNumMeshes < 1)
	{
		String errStr("The file " + a_File + " does not contain any meshes.");
		Utility::ThrowError(errStr);
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

	// Creating a mesh and adding sub meshes
	Mesh* retMesh = new Mesh(scene->mNumMeshes, scene->mNumAnimations);
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		aiMaterial* impMat = scene->mMaterials[scene->mMeshes[i]->mMaterialIndex];
		aiString str;
		impMat->Get(AI_MATKEY_NAME, str);
		String matName = str.C_Str();
		Material* meshMat = GetMaterial(matName);
		SubMesh* mesh = new SubMesh(*scene->mMeshes[i]);
		mesh->SetMaterial(meshMat);
		retMesh->AddSubMesh(mesh);
	}

	// Animation data (if it exists)
	if (scene->HasAnimations())
	{
		for (unsigned int i = 0; i < scene->mNumAnimations; i++)
		{
			MeshAnimation* animData = new MeshAnimation(*scene->mAnimations[i]);
			retMesh->AddAnimation(animData);
		}
	}

	AddMeshToMap(a_EntityName, retMesh);
	AddFileNameToMap(a_File, retMesh);
	return retMesh;
}

Lux::Core::Mesh* Lux::Core::Internal::ResourceHandlerOGL::CreateMeshFromMemory(FileInfo* a_Info, const String& a_EntityName, unsigned int a_PostProcessFlags)
{
	if (a_Info == nullptr)
	{
		Utility::ThrowError("Failed to create entity from Memory. The passed FileInfo pointer is NULL.");
	}

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFileFromMemory(a_Info->m_RawData, a_Info->m_DataLength, a_PostProcessFlags);
	if (scene == nullptr)
	{
		String errStr("Failed to read file. ");
		errStr.append(importer.GetErrorString());
		Utility::ThrowError(errStr);
	}

	if (scene->mNumMeshes < 1)
	{
		String errStr("The file does not contain any meshes.");
		Utility::ThrowError(errStr);
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
	Mesh* retEntity = new Mesh(scene->mNumMeshes, scene->mNumAnimations);
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		aiMaterial* impMat = scene->mMaterials[scene->mMeshes[i]->mMaterialIndex];
		aiString str;
		impMat->Get(AI_MATKEY_NAME, str);
		String matName = str.C_Str();
		Material* meshMat = GetMaterial(matName);
		SubMesh* mesh = new SubMesh(*scene->mMeshes[i]);
		mesh->SetMaterial(meshMat);
		retEntity->AddSubMesh(mesh);
	}

	// Animation data (if it exists)
	if (scene->HasAnimations())
	{
		for (unsigned int i = 0; i < scene->mNumAnimations; i++)
		{
			MeshAnimation* animData = new MeshAnimation(*scene->mAnimations[i]);
			retEntity->AddAnimation(animData);
		}
	}

	AddMeshToMap(a_EntityName, retEntity);
	return retEntity;
}

void Lux::Core::Internal::ResourceHandlerOGL::LoadAllTexturesOfTypeFromMaterial(aiMaterial* a_Mat, aiTextureType a_TexType)
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
			LUX_LOG(Utility::logWARNING) << "Could not find texture " << texName.C_Str();
			continue;
		}
		CreateTextureFromFile(String(texName.C_Str()), String(texName.C_Str()));
	}
}

Lux::Core::Texture* Lux::Core::Internal::ResourceHandlerOGL::CreateTextureFromFile(const String& a_File, const String& a_TexName)
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
			Utility::ThrowError(err);
		}
	}

	//Does the extension support reading?
	if (!FreeImage_FIFSupportsReading(imgFormat))
	{
		String err("The file " + a_File + " Could not be loaded. The file extension does not support reading.");
		Utility::ThrowError(err);
	}

	// Load the file in a bitmap
	FIBITMAP* bitmap = FreeImage_LoadFromMemory(imgFormat, freeImgMemoryPtr);
	FIBITMAP* convertedBitmap = FreeImage_ConvertTo32Bits(bitmap);

	if (convertedBitmap == nullptr)
	{
		String err("The file " + a_File + " Could not be loaded. Loading function returned NULL.");
		Utility::ThrowError(err);
	}

	unsigned int imgWidth = FreeImage_GetWidth(convertedBitmap);
	unsigned int imgHeight = FreeImage_GetHeight(convertedBitmap);
	unsigned char* bits = FreeImage_GetBits(convertedBitmap);

	// Normally this should never happen, but just to be safe
	if (imgWidth == 0 || imgHeight == 0 || bits == nullptr)
	{
		String err("The file " + a_File + " Could not be created. Failed to retrieve proper data from the loaded file.");
		Utility::ThrowError(err);
	}

	// Check if a texture with this name already exists
	if (TextureExists(a_TexName))
	{
		// Delete the old one
		DeleteTexture(a_TexName);
	}

	Internal::Texture2DOGL* tex2d = new Internal::Texture2DOGL(imgWidth, imgHeight, bits);

	// Put it in the map
	AddTextureToMap(a_TexName, tex2d);

	//Free FreeImage's copy of the data
	FreeImage_Unload(convertedBitmap);

	return tex2d;
}

Lux::Core::Texture* Lux::Core::Internal::ResourceHandlerOGL::CreateTextureFromMemory(FileInfo* a_Info, const String& a_TexName)
{
	FIMEMORY* freeImgMemoryPtr = nullptr;
	freeImgMemoryPtr = FreeImage_OpenMemory((unsigned char*)a_Info->m_RawData, a_Info->m_DataLength);
	FREE_IMAGE_FORMAT imgFormat = FreeImage_GetFileTypeFromMemory(freeImgMemoryPtr, a_Info->m_DataLength);

	if (imgFormat == FIF_UNKNOWN)
	{
		String err("The file from memory could not be loaded. Unknown format.");
		Utility::ThrowError(err);
	}

	//Does the extension support reading?
	if (!FreeImage_FIFSupportsReading(imgFormat))
	{
		String err("The file could not be loaded. The file extension does not support reading.");
		Utility::ThrowError(err);
	}

	// Load the file in a bitmap
	FIBITMAP* bitmap = FreeImage_LoadFromMemory(imgFormat, freeImgMemoryPtr);
	FIBITMAP* convertedBitmap = FreeImage_ConvertTo32Bits(bitmap);

	if (convertedBitmap == nullptr)
	{
		String err("The file could not be loaded. Loading function returned NULL.");
		Utility::ThrowError(err);
	}

	unsigned int imgWidth = FreeImage_GetWidth(convertedBitmap);
	unsigned int imgHeight = FreeImage_GetHeight(convertedBitmap);
	unsigned char* bits = FreeImage_GetBits(convertedBitmap);

	// Normally this should never happen, but just to be safe
	if (imgWidth == 0 || imgHeight == 0 || bits == nullptr)
	{
		String err("The file could not be created. Failed to retrieve proper data from the loaded file.");
		Utility::ThrowError(err);
	}

	// Check if a texture with this name already exists
	if (TextureExists(a_TexName))
	{
		// Delete the old one
		DeleteTexture(a_TexName);
	}

	Internal::Texture2DOGL* tex2d = new Internal::Texture2DOGL(imgWidth, imgHeight, bits);

	// Put it in the map
	AddTextureToMap(a_TexName, tex2d);

	//Free FreeImage's copy of the data
	FreeImage_Unload(convertedBitmap);

	return tex2d;
}

Lux::Core::Shader* Lux::Core::Internal::ResourceHandlerOGL::CreateShaderFromFile(const String& a_File, const String& a_ShaderName)
{
	FileInfo* file = FileHandler::GetInstance().LoadFileInMemory(a_File);
	// TODO
	String str(file->m_RawData, file->m_DataLength);
	yyscan_t scanner;
	LuxFileScannerlex_init(&scanner);
	LuxFileScannerlex_init_extra(a_File.c_str(), &scanner);
	LuxFileScanner_scan_string(str.c_str(), scanner);
	LuxFileScannerlex(scanner);
	LuxFileScannerlex_destroy(scanner);
	return nullptr;
}

#if LUX_THREAD_SAFE == TRUE
void Lux::Core::Internal::ResourceHandlerOGL::AddMeshToMap(const String& a_Str, Mesh* a_Ent)
{
	std::unique_lock<std::mutex> lock(m_MeshMapMutex); // Upon construction of the lock the mutex will be immediately locked
	m_MeshMap.insert(std::make_pair(Key(a_Str), std::shared_ptr<Mesh>(a_Ent)));
}
Lux::Core::Mesh* Lux::Core::Internal::ResourceHandlerOGL::GetMesh(const String& a_Name)
{
	std::unique_lock<std::mutex> lock(m_MeshMapMutex); // Upon construction of the lock the mutex will be immediately locked
	return m_MeshMap.at(Key(a_Name)).get();
}

void Lux::Core::Internal::ResourceHandlerOGL::AddMaterialToMap(const String& a_Str, Material* a_Mat)
{
	std::unique_lock<std::mutex> lock(m_MaterialMapMutex); // Upon construction of the lock the mutex will be immediately locked
	m_MaterialMap.insert(std::make_pair(Key(a_Str), std::shared_ptr<Material>(a_Mat)));
}

Lux::Core::Material* Lux::Core::Internal::ResourceHandlerOGL::GetMaterial(const String& a_Name)
{
	std::unique_lock<std::mutex> lock(m_MaterialMapMutex); // Upon construction of the lock the mutex will be immediately locked
	return m_MaterialMap.at(Key(a_Name)).get();
}

bool Lux::Core::Internal::ResourceHandlerOGL::MaterialExists(const String& a_Name)
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

bool Lux::Core::Internal::ResourceHandlerOGL::MeshExists(const String& a_Name)
{
	Key k(a_Name);
	std::unique_lock<std::mutex> lock(m_MeshMapMutex); // Upon construction of the lock the mutex will be immediately locked
	unsigned int count = m_MeshMap.count(k);

	if (count > 0)
	{
		return true;
	}

	return false;
}

bool Lux::Core::Internal::ResourceHandlerOGL::TextureExists(const String& a_Name)
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

Lux::Core::Texture* Lux::Core::Internal::ResourceHandlerOGL::GetTexture(const String& a_Name)
{
	std::unique_lock<std::mutex> lock(m_TextureMapMutex);
	return m_TextureMap.at(Key(a_Name)).get();
}

void Lux::Core::Internal::ResourceHandlerOGL::AddTextureToMap(const String& a_Str, Texture* a_Tex)
{
	std::unique_lock<std::mutex> lock(m_TextureMapMutex);
	m_TextureMap.insert(std::make_pair(Key(a_Str), std::shared_ptr<Texture>(a_Tex)));
}

bool Lux::Core::Internal::ResourceHandlerOGL::DeleteTexture(const String& a_Name)
{
	if (!TextureExists(a_Name))
	{
		LUX_LOG(Utility::logWARNING) << "Could not delete texture with name. " << a_Name << " Texture doesn't exist.";
		return false;
	}
	std::unique_lock<std::mutex> lock(m_TextureMapMutex);
	m_TextureMap.at(Key(a_Name)).reset();
	m_TextureMap.erase(Key(a_Name));

	return true;
}

void Lux::Core::Internal::ResourceHandlerOGL::AddFileNameToMap(const String& a_Str, Mesh* a_Ent)
{
	std::unique_lock<std::mutex> lock(m_MeshMapMutex);
	m_LoadedFilenameMeshes.insert(std::make_pair(Key(a_Str), std::shared_ptr<Mesh>(a_Ent)));
}

Lux::Core::Mesh* Lux::Core::Internal::ResourceHandlerOGL::GetLoadedMesh(const String& a_FileStr)
{
	std::unique_lock<std::mutex> lock(m_MeshMapMutex);
	MeshMap::iterator it = m_LoadedFilenameMeshes.find(Key(a_FileStr));

	if (it != m_LoadedFilenameMeshes.end())
	{
		return it->second.get();
	}

	return nullptr;
}

#else
void Lux::Core::Internal::ResourceHandlerOGL::AddMeshToMap(const String& a_Str, Mesh* a_Ent)
{
	m_MeshMap.insert(std::make_pair(Key(a_Str), std::shared_ptr<Mesh>(a_Ent)));
}

Lux::Core::Mesh* Lux::Core::Internal::ResourceHandlerOGL::GetMesh(const String& a_Name)
{
	return m_MeshMap.at(Key(a_Name)).get();
}

void Lux::Core::Internal::ResourceHandlerOGL::AddMaterialToMap(const String& a_Str, Material* a_Mat)
{
	m_MaterialMap.insert(std::make_pair(Key(a_Str), std::shared_ptr<Material>(a_Mat)));
}

Lux::Core::Material* Lux::Core::Internal::ResourceHandlerOGL::GetMaterial(const String& a_Name)
{
	return m_MaterialMap.at(Key(a_Name)).get();
}

bool Lux::Core::Internal::ResourceHandlerOGL::MaterialExists(const String& a_Name)
{
	Key k(a_Name);
	unsigned int count = m_MaterialMap.count(k);

	if (count > 0)
	{
		return true;
	}

	return false;
}

bool Lux::Core::Internal::ResourceHandlerOGL::MeshExists(const String& a_Name)
{
	Key k(a_Name);
	unsigned int count = m_MeshMap.count(k);

	if (count > 0)
	{
		return true;
	}

	return false;
}

bool Lux::Core::Internal::ResourceHandlerOGL::TextureExists(const String& a_Name)
{
	Key k(a_Name);
	unsigned int count = m_TextureMap.count(k);

	if (count > 0)
	{
		return true;
	}

	return false;
}

Lux::Core::Texture* Lux::Core::Internal::ResourceHandlerOGL::GetTexture(const String& a_Name)
{
	return m_TextureMap.at(Key(a_Name)).get();
}

void Lux::Core::Internal::ResourceHandlerOGL::AddTextureToMap(const String& a_Str, Texture* a_Tex)
{
	m_TextureMap.insert(std::make_pair(Key(a_Str), std::shared_ptr<Texture>(a_Tex)));
}

bool Lux::Core::Internal::ResourceHandlerOGL::DeleteTexture(const String& a_Name)
{
	if (!TextureExists(a_Name))
	{
		LUX_LOG(Utility::logWARNING) << "Could not delete texture with name. " << a_Name << " Texture doesn't exist.";
		return false;
	}
	m_TextureMap.at(Key(a_Name)).reset();
	m_TextureMap.erase(Key(a_Name));

	return true;
}

void Lux::Core::Internal::ResourceHandlerOGL::AddFileNameToMap(const String& a_Str, Mesh* a_Ent)
{
	m_LoadedFilenameMeshes.insert(std::make_pair(Key(a_Str), std::shared_ptr<Mesh>(a_Ent)));
}

Lux::Core::Mesh* Lux::Core::Internal::ResourceHandlerOGL::GetLoadedMesh(const String& a_FileStr)
{
	MeshMap::iterator it = m_LoadedFilenameMeshes.find(Key(a_FileStr));

	if (it != m_LoadedFilenameMeshes.end())
	{
		return it->second.get();
	}

	return nullptr;
}

#endif // LUX_THREAD_SAFE
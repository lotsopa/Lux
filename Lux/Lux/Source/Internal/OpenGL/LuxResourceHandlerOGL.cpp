#include "LuxPCH.h"
#include "LuxKey.h"
#include "LuxMaterial.h"
#include "LuxBufferOGL.h"
#include "LuxVertexArrayOGL.h"
#include "LuxSubMesh.h"
#include "LuxSubMeshOGL.h"
#include "LuxMesh.h"
#include "LuxMeshOGL.h"
#include "LuxMeshAnimation.h"
#include "LuxResourceHandler.h"
#include "LuxResourceHandlerOGL.h"
#include "LuxTexture.h"
#include "LuxTexture2D.h"
#include "LuxTexture2DOGL.h"
#include "LuxTexture1D.h"
#include "LuxTexture1DOGL.h"
#include "LuxTexture3D.h"
#include "LuxTexture3DOGL.h"
#include "LuxShader.h"
#include "LuxShaderOGL.h"
#include "LuxFileHandler.h"
#include "LuxShaderFileParser.h"
#include "LuxErrorCheckOGL.h"
#include "LuxTextureSampler.h"
#include "LuxTextureSamplerOGL.h"
#include "LuxPhysicsMaterial.h"

#ifndef YY_NO_UNISTD_H
#define YY_NO_UNISTD_H
#endif
extern "C"
{
#include "LuxFileScanner.h"
}

Lux::Core::Internal::ResourceHandlerOGL::~ResourceHandlerOGL()
{
	DeleteMap(m_MeshMap);
	DeleteMap(m_MaterialMap);
	DeleteMap(m_Texture1DMap);
	DeleteMap(m_Texture2DMap);
	DeleteMap(m_Texture3DMap);
	DeleteMap(m_ShaderMap);
	DeleteMap(m_SamplerMap);
	DeleteMap(m_PhysicsMaterialMap);
}

Lux::Core::Internal::ResourceHandlerOGL::ResourceHandlerOGL()
{
	LUX_LOG(Utility::logINFO) << "Resource Handler created successfully.";
}

Lux::Core::ObserverPtr<Lux::Core::Mesh> Lux::Core::Internal::ResourceHandlerOGL::CreateMeshFromFile(const String& a_File, const String& a_EntityName, unsigned int a_PostProcessFlags)
{
	// Have we already loaded this mesh before? If we have just return it.
	Mesh* loadedMesh = GetLoadedMesh(a_File);
	if (loadedMesh != nullptr)
	{
		return ObserverPtr<Mesh>(loadedMesh);
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
		AddResourceToMap(matName, myMat, m_MaterialMap);

		if (ResourceExists(matName, m_MaterialMap))
		{
			static int ctr = 0;
			matName.append(std::to_string(ctr));
			ctr++;
		}

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
	MeshOGL* retMesh = new MeshOGL(scene->mNumMeshes, scene->mNumAnimations);

	// Setup Bounding Box
	vec3 meshMin(1e34f);
	vec3 meshMax(-1e33f);
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		unsigned int vertexCount = scene->mMeshes[i]->mNumVertices;

		for (unsigned int j = 0; j < vertexCount; j++)
		{
			meshMin.x = std::min(meshMin.x, scene->mMeshes[i]->mVertices[j].x);
			meshMin.y = std::min(meshMin.y, scene->mMeshes[i]->mVertices[j].y);
			meshMin.z = std::min(meshMin.z, scene->mMeshes[i]->mVertices[j].z);

			meshMax.x = std::max(meshMax.x, scene->mMeshes[i]->mVertices[j].x);
			meshMax.y = std::max(meshMax.y, scene->mMeshes[i]->mVertices[j].y);
			meshMax.z = std::max(meshMax.z, scene->mMeshes[i]->mVertices[j].z);
		}
	}

	// Make sure we center the mesh at the origin to compute a proper bounding box
	vec3 difference;
	for (unsigned int i = 0; i < 3; i++)
	{
		float absMeshMin = fabs(meshMin[i]);
		float absMeshMax = fabs(meshMax[i]);
		if (absMeshMin > absMeshMax)
		{
			difference[i] = (absMeshMin - absMeshMax) / 2;
			meshMin[i] = 1e34f;
			meshMax[i] = -1e33f;
			for (unsigned int k = 0; k < scene->mNumMeshes; k++)
			{
				unsigned int vertexCount = scene->mMeshes[k]->mNumVertices;

				for (unsigned int j = 0; j < vertexCount; j++)
				{
					scene->mMeshes[k]->mVertices[j][i] += difference[i];
					meshMin[i] = std::min(meshMin[i], scene->mMeshes[k]->mVertices[j][i]);
					meshMax[i] = std::max(meshMax[i], scene->mMeshes[k]->mVertices[j][i]);
				}
			}
		}
		else if (absMeshMin < absMeshMax)
		{
			difference[i] = (absMeshMax - absMeshMin) / 2;

			meshMin[i] = 1e34f;
			meshMax[i] = -1e33f;
			for (unsigned int k = 0; k < scene->mNumMeshes; k++)
			{
				unsigned int vertexCount = scene->mMeshes[k]->mNumVertices;

				for (unsigned int j = 0; j < vertexCount; j++)
				{
					scene->mMeshes[k]->mVertices[j][i] -= difference[i];
					meshMin[i] = std::min(meshMin[i], scene->mMeshes[k]->mVertices[j][i]);
					meshMax[i] = std::max(meshMax[i], scene->mMeshes[k]->mVertices[j][i]);
				}
			}
		}
	}

	AABB& aabb = retMesh->GetAABB();
	aabb.SetMax(meshMax);
	aabb.SetMin(meshMin);

	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		SubMeshOGL* mesh = new SubMeshOGL(*scene->mMeshes[i]);

		// Set mesh material and textures
		aiMaterial* impMat = scene->mMaterials[scene->mMeshes[i]->mMaterialIndex];
		aiString str;
		impMat->Get(AI_MATKEY_NAME, str);
		String matName = str.C_Str();
		mesh->SetMaterialProperties(GetResource(matName, m_MaterialMap));
		AssignLoadedTexturesToSubMesh(mesh, impMat);
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

	AddResourceToMap(a_EntityName, (Mesh*)retMesh, m_MeshMap);
	AddFileNameToMap(a_File, retMesh);
	Utility::SafePtrDelete(file);
	return ObserverPtr<Mesh>(retMesh);
}

Lux::Core::ObserverPtr<Lux::Core::Mesh> Lux::Core::Internal::ResourceHandlerOGL::CreateMeshFromMemory(FileInfo* a_Info, const String& a_EntityName, unsigned int a_PostProcessFlags)
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

		if (ResourceExists(matName, m_MaterialMap))
		{
			static int ctr = 0;
			matName.append(std::to_string(ctr));
			ctr++;
		}

		AddResourceToMap(matName, myMat, m_MaterialMap);

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
	MeshOGL* retEntity = new MeshOGL(scene->mNumMeshes, scene->mNumAnimations);

	// Setup Bounding Box
	vec3 meshMin(1e34f);
	vec3 meshMax(-1e33f);
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		unsigned int vertexCount = scene->mMeshes[i]->mNumVertices;

		for (unsigned int j = 0; j < vertexCount; j++)
		{
			meshMin.x = std::min(meshMin.x, scene->mMeshes[i]->mVertices[j].x);
			meshMin.y = std::min(meshMin.y, scene->mMeshes[i]->mVertices[j].y);
			meshMin.z = std::min(meshMin.z, scene->mMeshes[i]->mVertices[j].z);

			meshMax.x = std::max(meshMax.x, scene->mMeshes[i]->mVertices[j].x);
			meshMax.y = std::max(meshMax.y, scene->mMeshes[i]->mVertices[j].y);
			meshMax.z = std::max(meshMax.z, scene->mMeshes[i]->mVertices[j].z);
		}
	}

	// Make sure we center the mesh at the origin to compute a proper bounding box
	vec3 difference;
	for (unsigned int i = 0; i < 3; i++)
	{
		float absMeshMin = fabs(meshMin[i]);
		float absMeshMax = fabs(meshMax[i]);
		if (absMeshMin > absMeshMax)
		{
			difference[i] = (absMeshMin - absMeshMax) / 2;
			meshMin[i] = 1e34f;
			meshMax[i] = -1e33f;
			for (unsigned int k = 0; k < scene->mNumMeshes; k++)
			{
				unsigned int vertexCount = scene->mMeshes[k]->mNumVertices;

				for (unsigned int j = 0; j < vertexCount; j++)
				{
					scene->mMeshes[k]->mVertices[j][i] += difference[i];
					meshMin[i] = std::min(meshMin[i], scene->mMeshes[k]->mVertices[j][i]);
					meshMax[i] = std::max(meshMax[i], scene->mMeshes[k]->mVertices[j][i]);
				}
			}
		}
		else if (absMeshMin < absMeshMax)
		{
			difference[i] = (absMeshMax - absMeshMin) / 2;

			meshMin[i] = 1e34f;
			meshMax[i] = -1e33f;
			for (unsigned int k = 0; k < scene->mNumMeshes; k++)
			{
				unsigned int vertexCount = scene->mMeshes[k]->mNumVertices;

				for (unsigned int j = 0; j < vertexCount; j++)
				{
					scene->mMeshes[k]->mVertices[j][i] -= difference[i];
					meshMin[i] = std::min(meshMin[i], scene->mMeshes[k]->mVertices[j][i]);
					meshMax[i] = std::max(meshMax[i], scene->mMeshes[k]->mVertices[j][i]);
				}
			}
		}
	}

	AABB& aabb = retEntity->GetAABB();
	aabb.SetMax(meshMax);
	aabb.SetMin(meshMin);

	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		SubMeshOGL* mesh = new SubMeshOGL(*scene->mMeshes[i]);

		// Set mesh material and textures
		aiMaterial* impMat = scene->mMaterials[scene->mMeshes[i]->mMaterialIndex];
		aiString str;
		impMat->Get(AI_MATKEY_NAME, str);
		String matName = str.C_Str();
		mesh->SetMaterialProperties(GetResource(matName, m_MaterialMap));
		AssignLoadedTexturesToSubMesh(mesh, impMat);
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

	AddResourceToMap(a_EntityName, (Mesh*)retEntity, m_MeshMap);
	return ObserverPtr<Mesh>(retEntity);
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
		CreateTexture2DFromFile(String(texName.C_Str()), String(texName.C_Str()));
	}
}

Lux::Core::ObserverPtr<Lux::Core::Texture2D> Lux::Core::Internal::ResourceHandlerOGL::CreateTexture2DFromFile(const String& a_File, const String& a_TexName, const String& a_SamplerName /*= String("LuxDefaultSampler")*/)
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
	FreeImage_Unload(bitmap);
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
	if (Texture2DExists(a_TexName))
	{
		// Delete the old one
		DeleteTexture2D(a_TexName);
	}

	Internal::Texture2DOGL* tex2d = new Internal::Texture2DOGL(imgWidth, imgHeight, bits);

	// Put it in the map
	tex2d->SetSampler(GetTextureSampler(a_SamplerName));
	AddResourceToMap(a_TexName, (Texture2D*)tex2d, m_Texture2DMap);

	//Free FreeImage's copy of the data
	FreeImage_Unload(convertedBitmap);
	Utility::SafePtrDelete(file);
	return ObserverPtr<Texture2D>(tex2d);
}

Lux::Core::ObserverPtr<Lux::Core::Texture2D> Lux::Core::Internal::ResourceHandlerOGL::CreateTexture2DFromMemory(FileInfo* a_Info, const String& a_TexName, const String& a_SamplerName /*= String("LuxDefaultSampler")*/)
{
	FIMEMORY* freeImgMemoryPtr = nullptr;
	freeImgMemoryPtr = FreeImage_OpenMemory((unsigned char*)a_Info->m_RawData, a_Info->m_DataLength);
	FREE_IMAGE_FORMAT imgFormat = FreeImage_GetFileTypeFromMemory(freeImgMemoryPtr, a_Info->m_DataLength);

	if (imgFormat == FIF_UNKNOWN)
	{
		imgFormat = FreeImage_GetFIFFromFilename(a_TexName.c_str());

		// If it's still unknown, throw
		if (imgFormat == FIF_UNKNOWN)
		{
			String err("The file Could not be loaded. Unknown format.");
			Utility::ThrowError(err);
		}
	}

	//Does the extension support reading?
	if (!FreeImage_FIFSupportsReading(imgFormat))
	{
		String err("The file Could not be loaded. The file extension does not support reading.");
		Utility::ThrowError(err);
	}

	// Load the file in a bitmap
	FIBITMAP* bitmap = FreeImage_LoadFromMemory(imgFormat, freeImgMemoryPtr);
	FIBITMAP* convertedBitmap = FreeImage_ConvertTo32Bits(bitmap);
	FreeImage_Unload(bitmap);
	if (convertedBitmap == nullptr)
	{
		String err("The file Could not be loaded. Loading function returned NULL.");
		Utility::ThrowError(err);
	}

	unsigned int imgWidth = FreeImage_GetWidth(convertedBitmap);
	unsigned int imgHeight = FreeImage_GetHeight(convertedBitmap);
	unsigned char* bits = FreeImage_GetBits(convertedBitmap);

	// Normally this should never happen, but just to be safe
	if (imgWidth == 0 || imgHeight == 0 || bits == nullptr)
	{
		String err("The file Could not be created. Failed to retrieve proper data from the loaded file.");
		Utility::ThrowError(err);
	}

	// Check if a texture with this name already exists
	if (Texture2DExists(a_TexName))
	{
		// Delete the old one
		DeleteTexture2D(a_TexName);
	}

	Internal::Texture2DOGL* tex2d = new Internal::Texture2DOGL(imgWidth, imgHeight, bits);

	// Put it in the map
	tex2d->SetSampler(GetTextureSampler(a_SamplerName));
	AddResourceToMap(a_TexName, (Texture2D*)tex2d, m_Texture2DMap);

	//Free FreeImage's copy of the data
	FreeImage_Unload(convertedBitmap);
	Utility::SafePtrDelete(a_Info);
	return ObserverPtr<Texture2D>(tex2d);
}

Lux::Core::ObserverPtr<Lux::Core::Texture1D> Lux::Core::Internal::ResourceHandlerOGL::CreateTexture1DFromFile(const String& a_File, const String& a_TexName, const String& a_SamplerName /*= String("LuxDefaultSampler")*/)
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
	FreeImage_Unload(bitmap);
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
	if (Texture1DExists(a_TexName))
	{
		// Delete the old one
		DeleteTexture1D(a_TexName);
	}

	Internal::Texture1DOGL* tex1d = new Internal::Texture1DOGL(imgWidth, bits);

	// Put it in the map
	tex1d->SetSampler(GetTextureSampler(a_SamplerName));
	AddResourceToMap(a_TexName, (Texture1D*)tex1d, m_Texture1DMap);

	//Free FreeImage's copy of the data
	FreeImage_Unload(convertedBitmap);
	Utility::SafePtrDelete(file);
	return ObserverPtr<Texture1D>(tex1d);
}

Lux::Core::ObserverPtr<Lux::Core::Texture1D> Lux::Core::Internal::ResourceHandlerOGL::CreateTexture1DFromMemory(FileInfo* a_Info, const String& a_TexName, const String& a_SamplerName /*= String("LuxDefaultSampler")*/)
{
	FIMEMORY* freeImgMemoryPtr = nullptr;
	freeImgMemoryPtr = FreeImage_OpenMemory((unsigned char*)a_Info->m_RawData, a_Info->m_DataLength);
	FREE_IMAGE_FORMAT imgFormat = FreeImage_GetFileTypeFromMemory(freeImgMemoryPtr, a_Info->m_DataLength);

	if (imgFormat == FIF_UNKNOWN)
	{
		imgFormat = FreeImage_GetFIFFromFilename(a_TexName.c_str());

		// If it's still unknown, throw
		if (imgFormat == FIF_UNKNOWN)
		{
			String err("The file Could not be loaded. Unknown format.");
			Utility::ThrowError(err);
		}
	}

	//Does the extension support reading?
	if (!FreeImage_FIFSupportsReading(imgFormat))
	{
		String err("The file Could not be loaded. The file extension does not support reading.");
		Utility::ThrowError(err);
	}

	// Load the file in a bitmap
	FIBITMAP* bitmap = FreeImage_LoadFromMemory(imgFormat, freeImgMemoryPtr);
	FIBITMAP* convertedBitmap = FreeImage_ConvertTo32Bits(bitmap);
	FreeImage_Unload(bitmap);
	if (convertedBitmap == nullptr)
	{
		String err("The file Could not be loaded. Loading function returned NULL.");
		Utility::ThrowError(err);
	}

	unsigned int imgWidth = FreeImage_GetWidth(convertedBitmap);
	unsigned int imgHeight = FreeImage_GetHeight(convertedBitmap);
	unsigned char* bits = FreeImage_GetBits(convertedBitmap);

	// Normally this should never happen, but just to be safe
	if (imgWidth == 0 || imgHeight == 0 || bits == nullptr)
	{
		String err("The file Could not be created. Failed to retrieve proper data from the loaded file.");
		Utility::ThrowError(err);
	}

	// Check if a texture with this name already exists
	if (Texture1DExists(a_TexName))
	{
		// Delete the old one
		DeleteTexture1D(a_TexName);
	}

	Internal::Texture1DOGL* tex1d = new Internal::Texture1DOGL(imgWidth, bits);

	// Put it in the map
	tex1d->SetSampler(GetTextureSampler(a_SamplerName));
	AddResourceToMap(a_TexName, (Texture1D*)tex1d, m_Texture1DMap);

	//Free FreeImage's copy of the data
	FreeImage_Unload(convertedBitmap);
	Utility::SafePtrDelete(a_Info);
	return ObserverPtr<Texture1D>(tex1d);
}

Lux::Core::ObserverPtr<Lux::Core::Texture3D> Lux::Core::Internal::ResourceHandlerOGL::CreateTexture3DFromFile(const String& a_File, const String& a_TexName, const String& a_SamplerName /*= String("LuxDefaultSampler")*/)
{
	// TODO
	return nullptr;
}

Lux::Core::ObserverPtr<Lux::Core::Texture3D> Lux::Core::Internal::ResourceHandlerOGL::CreateTexture3DFromMemory(FileInfo* a_Info, const String& a_TexName, const String& a_SamplerName /*= String("LuxDefaultSampler")*/)
{
	// TODO
	return nullptr;
}

Lux::Core::ObserverPtr<Lux::Core::Shader> Lux::Core::Internal::ResourceHandlerOGL::CreateShaderFromFile(const String& a_File, const String& a_ShaderName)
{
	FileHandler& fileHandler = FileHandler::GetInstance();
	FileInfo* file = fileHandler.LoadFileInMemory(a_File);
	String str(file->m_RawData, file->m_DataLength);
	Utility::SafePtrDelete(file);

	// Tokenize file
	yyscan_t scanner;
	LuxFileScannerlex_init_extra(a_File.c_str(), &scanner);
	LuxFileScanner_scan_string(str.c_str(), scanner);

	// Create a new parser
	fileHandler.CreateShaderParser(Key(a_File));
	LuxFileScannerlex(scanner);
	LuxFileScannerlex_destroy(scanner);

	ShaderFileParser& shaderParser = fileHandler.GetShaderParser(Key(a_File));
	std::vector<unsigned int> loadedShaders;
	
	for (unsigned int i = 0; i < NUM_SHADER_PROGRAMS; i++)
	{
		String fileName = shaderParser.GetParsedProgramGLSL((ShaderProgram)i);

		if (fileName.empty())
			continue;

		FileInfo* shaderInfo = fileHandler.LoadFileInMemory(fileName);
		
		unsigned int loadedShaderHandle = 0;

		switch (i)
		{
		case VERTEX_PROGRAM:
			loadedShaderHandle = LoadOGLShader(GL_VERTEX_SHADER, shaderInfo);
			break;

		case FRAGMENT_PROGRAM:
			loadedShaderHandle = LoadOGLShader(GL_FRAGMENT_SHADER, shaderInfo);
			break;

		case GEOMETRY_PROGRAM:
			loadedShaderHandle = LoadOGLShader(GL_GEOMETRY_SHADER, shaderInfo);
			break;
		}
		loadedShaders.push_back(loadedShaderHandle);
		Utility::SafePtrDelete(shaderInfo);
	}

	if (loadedShaders.empty())
	{
		Utility::ThrowError("Failed to load shader. File is empty.");
	}

	// Create a Shader Object
	ShaderOGL* shader = new ShaderOGL(loadedShaders);
	AddResourceToMap(a_ShaderName, (Shader*)shader, m_ShaderMap);
	fileHandler.DestroyShaderParser(Key(a_File));
	return ObserverPtr<Shader>(shader);
}


unsigned int Lux::Core::Internal::ResourceHandlerOGL::LoadOGLShader(GLenum a_ShaderType, FileInfo* a_FileInfo)
{
	// Create a shader object.
	GLuint shader = glCreateShader(a_ShaderType);
	String source(a_FileInfo->m_RawData, a_FileInfo->m_DataLength);
	// Load the shader source for each shader object.
	const GLchar* sources[] = { source.c_str() };
	glShaderSource( shader, 1, sources, NULL );

	// Compile the shader.
	glCompileShader( shader );

	// Check for errors
	GLint compileStatus;
	glGetShaderiv( shader, GL_COMPILE_STATUS, &compileStatus ); 
	if ( compileStatus != GL_TRUE )
	{
		GLint logLength;
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logLength );
		GLchar* infoLog = new GLchar[logLength];
		glGetShaderInfoLog( shader, logLength, NULL, infoLog );

#ifdef _WIN32
		OutputDebugString(infoLog);
#else
		std::cerr << infoLog << std::endl;
#endif
		std::string errStr("Error Loading shader.");
		errStr.append(infoLog);
		delete infoLog;
		Utility::ThrowError(errStr);
		return 0;
	}

	return shader;
}

Lux::Core::ObserverPtr<Lux::Core::Material> Lux::Core::Internal::ResourceHandlerOGL::CreateMaterial(const String& a_Name)
{
	Material* mat = new Material();
	AddResourceToMap(a_Name, mat, m_MaterialMap);
	return ObserverPtr<Material>(mat);
}

void Lux::Core::Internal::ResourceHandlerOGL::LoadImageData(const String& a_Path, unsigned int& outWidth, unsigned int& outHeight, unsigned char* outData)
{
	FileInfo* file = FileHandler::GetInstance().LoadFileInMemory(a_Path);
	LoadImageData(file, outWidth, outHeight, outData);
}

void Lux::Core::Internal::ResourceHandlerOGL::LoadImageData( FileInfo* a_File, unsigned int& outWidth, unsigned int& outHeight, unsigned char* outData )
{
	FIMEMORY* freeImgMemoryPtr = nullptr;
	freeImgMemoryPtr = FreeImage_OpenMemory((unsigned char*)a_File->m_RawData, a_File->m_DataLength);
	FREE_IMAGE_FORMAT imgFormat = FreeImage_GetFileTypeFromMemory(freeImgMemoryPtr, a_File->m_DataLength);

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

	outWidth = FreeImage_GetWidth(convertedBitmap);
	outHeight = FreeImage_GetHeight(convertedBitmap);
	outData = FreeImage_GetBits(convertedBitmap);

	// Normally this should never happen, but just to be safe
	if (outWidth == 0 || outHeight == 0 || outData == nullptr)
	{
		String err("The file could not be created. Failed to retrieve proper data from the loaded file.");
		Utility::ThrowError(err);
	}

	//Free FreeImage's copy of the data
	FreeImage_Unload(convertedBitmap);
	Utility::SafePtrDelete(a_File);
}


Lux::Core::ObserverPtr<Lux::Core::Mesh> Lux::Core::Internal::ResourceHandlerOGL::GetMesh(const String& a_Name)
{
	return GetResource(a_Name, m_MeshMap);
}

Lux::Core::ObserverPtr<Lux::Core::Material> Lux::Core::Internal::ResourceHandlerOGL::GetMaterial(const String& a_Name)
{
	return GetResource(a_Name, m_MaterialMap);
}

bool Lux::Core::Internal::ResourceHandlerOGL::MaterialExists(const String& a_Name)
{
	return ResourceExists(a_Name, m_MaterialMap);
}

bool Lux::Core::Internal::ResourceHandlerOGL::MeshExists(const String& a_Name)
{
	return ResourceExists(a_Name, m_MeshMap);
}

bool Lux::Core::Internal::ResourceHandlerOGL::Texture2DExists(const String& a_Name)
{
	return ResourceExists(a_Name, m_Texture2DMap);
}

Lux::Core::ObserverPtr<Lux::Core::Texture2D> Lux::Core::Internal::ResourceHandlerOGL::GetTexture2D(const String& a_Name)
{
	return GetResource(a_Name, m_Texture2DMap);
}


bool Lux::Core::Internal::ResourceHandlerOGL::DeleteTexture2D(const String& a_Name)
{
	return DeleteResource(a_Name, m_Texture2DMap);
}

void Lux::Core::Internal::ResourceHandlerOGL::AddFileNameToMap(const String& a_Str, Mesh* a_Ent)
{
	m_LoadedFilenameMeshes.insert(std::make_pair(Key(a_Str),a_Ent));
}

Lux::Core::Mesh* Lux::Core::Internal::ResourceHandlerOGL::GetLoadedMesh(const String& a_FileStr)
{
	MeshMapSimple::iterator it = m_LoadedFilenameMeshes.find(Key(a_FileStr));

	if (it != m_LoadedFilenameMeshes.end())
	{
		return it->second;
	}

	return nullptr;
}

bool Lux::Core::Internal::ResourceHandlerOGL::ShaderExists(const String& a_Name)
{
	return ResourceExists(a_Name, m_ShaderMap);
}

Lux::Core::ObserverPtr<Lux::Core::Shader> Lux::Core::Internal::ResourceHandlerOGL::GetShader(const String& a_Name)
{
	return GetResource(a_Name , m_ShaderMap);
}

Lux::Core::ObserverPtr<Lux::Core::Texture3D> Lux::Core::Internal::ResourceHandlerOGL::GetTexture3D(const String& a_Name)
{
	return GetResource(a_Name, m_Texture3DMap);
}

Lux::Core::ObserverPtr<Lux::Core::Texture1D> Lux::Core::Internal::ResourceHandlerOGL::GetTexture1D(const String& a_Name)
{
	return GetResource(a_Name, m_Texture1DMap);
}

bool Lux::Core::Internal::ResourceHandlerOGL::Texture1DExists(const String& a_Name)
{
	return ResourceExists(a_Name, m_Texture1DMap);
}

bool Lux::Core::Internal::ResourceHandlerOGL::Texture3DExists(const String& a_Name)
{
	return ResourceExists(a_Name, m_Texture3DMap);
}

bool Lux::Core::Internal::ResourceHandlerOGL::DeleteTexture1D(const String& a_Name)
{
	return DeleteResource(a_Name, m_Texture1DMap);
}

bool Lux::Core::Internal::ResourceHandlerOGL::DeleteTexture3D(const String& a_Name)
{
	return DeleteResource(a_Name, m_Texture3DMap);
}


Lux::Core::ObserverPtr<Lux::Core::TextureSampler> Lux::Core::Internal::ResourceHandlerOGL::CreateTextureSampler(const String& a_Name, TextureSamplerOptions& a_InitOptions)
{
	TextureSamplerOGL* sampler = new TextureSamplerOGL(a_InitOptions);
	AddResourceToMap(a_Name, (TextureSampler*)sampler, m_SamplerMap);
	return ObserverPtr<TextureSampler>(sampler);
}

Lux::Core::ObserverPtr<Lux::Core::TextureSampler> Lux::Core::Internal::ResourceHandlerOGL::GetTextureSampler(const String& a_Name)
{
	return GetResource(a_Name, m_SamplerMap);
}

bool Lux::Core::Internal::ResourceHandlerOGL::DeleteTextureSampler(const String& a_Name)
{
	return DeleteResource(a_Name, m_SamplerMap);
}

bool Lux::Core::Internal::ResourceHandlerOGL::TextureSamplerExists(const String& a_Name)
{
	return ResourceExists(a_Name, m_SamplerMap);
}

Lux::Core::ObserverPtr<Lux::Core::PhysicsMaterial> Lux::Core::Internal::ResourceHandlerOGL::CreatePhysicsMaterial(const String& a_Name, float a_Restitution /*= 0.0f*/, float a_DynamicFriction /*= 0.0f*/, float a_StaticFriction /*= 0.0f*/)
{
	PhysicsMaterial* material = new PhysicsMaterial(a_Restitution, a_DynamicFriction, a_StaticFriction);
	AddResourceToMap(a_Name, material, m_PhysicsMaterialMap);
	return ObserverPtr<PhysicsMaterial>(material);
}

Lux::Core::ObserverPtr<Lux::Core::PhysicsMaterial> Lux::Core::Internal::ResourceHandlerOGL::GetPhysicsMaterial(const String& a_Name)
{
	return GetResource(a_Name, m_PhysicsMaterialMap);
}

bool Lux::Core::Internal::ResourceHandlerOGL::PhysicsMaterialExists(const String& a_Name)
{
	return ResourceExists(a_Name, m_PhysicsMaterialMap);
}

bool Lux::Core::Internal::ResourceHandlerOGL::DeletePhysicsMaterial(const String& a_Name)
{
	return DeleteResource(a_Name, m_PhysicsMaterialMap);
}

void Lux::Core::Internal::ResourceHandlerOGL::AssignLoadedTexturesToSubMesh(SubMesh* a_SubMesh, aiMaterial* a_Mat)
{
	for (unsigned int i = 0; i < LUX_NUM_TEXTURES_PER_MESH; i++)
	{
		aiString texName;
		aiReturn texFound = a_Mat->GetTexture((aiTextureType)(i + 1), 0, &texName);
		if (texFound == AI_SUCCESS)
		{
			a_SubMesh->SetTexture((TextureIndex)(i), GetResource(String(texName.C_Str()), m_Texture2DMap));
		}
		else
			a_SubMesh->SetTexture((TextureIndex)(i), GetResource(LUX_DEFAULT_TEX, m_Texture2DMap));
	}
}

#include "LuxPCH.h"
#include "LuxKey.h"
#include "LuxMaterial.h"
#include "LuxSubMesh.h"
#include "LuxMesh.h"
#include "LuxMeshAnimation.h"
#include "LuxPhysicsMaterial.h"
#include "LuxResourceHandler.h"
#include "LuxResourceHandlerDX11.h"
#include "LuxTexture.h"
#include "LuxShader.h"
#include "LuxFileHandler.h"
#include "LuxShaderFileParser.h"
#include "LuxRenderWindow.h"
#include "LuxRenderWindowDX11.h"
#include "LuxMeshDX11.h"
#include "LuxSubMeshDX11.h"
#include "LuxTexture.h"
#include "LuxTexture2D.h"
#include "LuxShaderDX11.h"
#include "LuxTexture2DDX11.h"
#include "LuxTexture1D.h"
#include "LuxTexture1DDX11.h"
#include "LuxTexture3D.h"
#include "LuxTexture3DDX11.h"
#include "LuxMeshPostProcess.h"
#include "LuxTextureSampler.h"
#include "LuxTextureSamplerDX11.h"

#ifndef YY_NO_UNISTD_H
#define YY_NO_UNISTD_H
#endif
extern "C"
{
#include "LuxFileScanner.h"
}

Lux::Core::Internal::ResourceHandlerDX11::~ResourceHandlerDX11()
{
	DeleteMap(m_MeshMap);
	DeleteMap(m_MaterialMap);
	DeleteMap(m_Texture1DMap);
	DeleteMap(m_Texture2DMap);
	DeleteMap(m_Texture3DMap);
	DeleteMap(m_ShaderMap);
	DeleteMap(m_SamplerMap);
	DeleteMap(m_PhysicsMaterialMap);

	InputLayoutMap::iterator it;

	for (it = m_InputLayouts.begin(); it != m_InputLayouts.end(); ++it)
	{
		it->second.Reset();
	}
	m_InputLayouts.clear();
}

Lux::Core::ObserverPtr<Lux::Core::Mesh> Lux::Core::Internal::ResourceHandlerDX11::CreateMeshFromFile(const String& a_File, const String& a_EntityName, unsigned int a_PostProcessFlags)
{
	// Have we already loaded this mesh before? If we have, just return it.
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

	// Creating a mesh and adding sub meshes
	MeshDX11* retMesh = new MeshDX11(scene->mNumMeshes, scene->mNumAnimations);

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
		SubMeshDX11* mesh = new SubMeshDX11(*scene->mMeshes[i], m_RenderWindow->GetDeviceContextPtr());

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

Lux::Core::ObserverPtr<Lux::Core::Mesh> Lux::Core::Internal::ResourceHandlerDX11::CreateMeshFromMemory(FileInfo* a_Info, const String& a_EntityName, unsigned int a_PostProcessFlags)
{
	if (a_Info == nullptr)
	{
		Utility::ThrowError("Failed to create mesh from Memory. The passed FileInfo pointer is NULL.");
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
	MeshDX11* retEntity = new MeshDX11(scene->mNumMeshes, scene->mNumAnimations);

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
		SubMeshDX11* mesh = new SubMeshDX11(*scene->mMeshes[i], m_RenderWindow->GetDeviceContextPtr());

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

void Lux::Core::Internal::ResourceHandlerDX11::LoadAllTexturesOfTypeFromMaterial(aiMaterial* a_Mat, aiTextureType a_TexType)
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

Lux::Core::ObserverPtr<Lux::Core::Texture2D> Lux::Core::Internal::ResourceHandlerDX11::CreateTexture2DFromFile(const String& a_File, const String& a_TexName, const String& a_SamplerName /*= String("LuxDefaultSampler")*/)
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

	Internal::Texture2DDX11* tex2d = new Internal::Texture2DDX11(m_RenderWindow->GetDeviceContextPtr(), imgWidth, imgHeight, bits);

	// Put it in the map
	tex2d->SetSampler(GetTextureSampler(a_SamplerName));
	AddResourceToMap(a_TexName, (Texture2D*)tex2d, m_Texture2DMap);

	//Free FreeImage's copy of the data
	FreeImage_Unload(convertedBitmap);
	Utility::SafePtrDelete(file);
	return ObserverPtr<Texture2D>(tex2d);
}

Lux::Core::ObserverPtr<Lux::Core::Texture2D> Lux::Core::Internal::ResourceHandlerDX11::CreateTexture2DFromMemory(FileInfo* a_Info, const String& a_TexName, const String& a_SamplerName /*= String("LuxDefaultSampler")*/)
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

	Internal::Texture2DDX11* tex2d = new Internal::Texture2DDX11(m_RenderWindow->GetDeviceContextPtr(), imgWidth, imgHeight, bits);

	// Put it in the map
	tex2d->SetSampler(GetTextureSampler(a_SamplerName));
	AddResourceToMap(a_TexName, (Texture2D*)tex2d, m_Texture2DMap);

	//Free FreeImage's copy of the data
	FreeImage_Unload(convertedBitmap);
	Utility::SafePtrDelete(a_Info);
	return ObserverPtr<Texture2D>(tex2d);
}

Lux::Core::ObserverPtr<Lux::Core::Shader> Lux::Core::Internal::ResourceHandlerDX11::CreateShaderFromFile(const String& a_File, const String& a_ShaderName)
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
	
	ID3DBlob* errorMessage = nullptr;
	std::vector<DX11CompiledShader> loadedShaders;
	HRESULT result;
	for (unsigned int i = 0; i < NUM_SHADER_PROGRAMS; i++)
	{
		String fileName = shaderParser.GetParsedProgramHLSL((ShaderProgram)i);

		if (fileName.empty())
			continue;

		FileInfo* shaderInfo = fileHandler.LoadFileInMemory(fileName);

		ID3DBlob* blob = nullptr;
		DX11CompiledShader compiledShader;
		switch (i)
		{
		case VERTEX_PROGRAM:
		{
			result = D3DCompile(shaderInfo->m_RawData, shaderInfo->m_DataLength, NULL, NULL, NULL, "main", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, NULL,
				&blob, &errorMessage);

			if (FAILED(result))
			{
				Utility::ThrowError(String((char*)errorMessage->GetBufferPointer(), errorMessage->GetBufferSize()));
			}

			ID3D11VertexShader* vertshader = nullptr;
			result = m_RenderWindow->GetDevicePtr()->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, &vertshader);
			if (FAILED(result))
			{
				Utility::ThrowError("Error Compiling Vertex Shader: " + fileName);
			}
			compiledShader.m_Shader = vertshader;
			compiledShader.m_Type = VERTEX_PROGRAM;
			// Create Input Layout
			ID3D11InputLayout* inputLayout = nullptr;
			result = CreateInputLayoutDescFromVertexShaderSignature(blob, m_RenderWindow->GetDevicePtr(), &inputLayout);

			if (FAILED(result))
			{
				Utility::ThrowError("Could not create Vertex Shader Input Layout from the shader " + a_ShaderName);
			}

			AddInputLayoutToMap(a_ShaderName, inputLayout);
			compiledShader.m_InputLayout = inputLayout;
		}
			break;

		case FRAGMENT_PROGRAM:
		{
			result = D3DCompile(shaderInfo->m_RawData, shaderInfo->m_DataLength, NULL, NULL, NULL, "main", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, NULL,
				&blob, &errorMessage);

			if (FAILED(result))
			{
				Utility::ThrowError(String((char*)errorMessage->GetBufferPointer(), errorMessage->GetBufferSize()));
			}

			ID3D11PixelShader* pixshader = nullptr;
			result = m_RenderWindow->GetDevicePtr()->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, &pixshader);
			if (FAILED(result))
			{
				Utility::ThrowError("Error Compiling Fragment Shader: " + fileName);
			}
			compiledShader.m_Shader = pixshader;
			compiledShader.m_Type = FRAGMENT_PROGRAM;
		}
			break;

		case GEOMETRY_PROGRAM:
		{
			result = D3DCompile(shaderInfo->m_RawData, shaderInfo->m_DataLength, NULL, NULL, NULL, "main", "gs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, NULL,
				&blob, &errorMessage);

			if (FAILED(result))
			{
				Utility::ThrowError(String((char*)errorMessage->GetBufferPointer(), errorMessage->GetBufferSize()));
			}

			ID3D11GeometryShader* geomshader = nullptr;
			result = m_RenderWindow->GetDevicePtr()->CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, &geomshader);
			if (FAILED(result))
			{
				Utility::ThrowError("Error Compiling Geometry Shader: " + fileName);
			}
			compiledShader.m_Shader = geomshader;
			compiledShader.m_Type = GEOMETRY_PROGRAM;
		}
			break;
		}
		compiledShader.m_CompileBlob = blob;
		loadedShaders.push_back(compiledShader);
		Utility::SafePtrDelete(shaderInfo);
	}

	// Create a Shader Object
	ShaderDX11* shader = new ShaderDX11(loadedShaders, m_RenderWindow->GetDeviceContextPtr());
	AddShaderToMap(a_ShaderName, shader);

	fileHandler.DestroyShaderParser(Key(a_File));
	return ObserverPtr<Shader>(shader);
}

Lux::Core::ObserverPtr<Lux::Core::Material> Lux::Core::Internal::ResourceHandlerDX11::CreateMaterial(const String& a_Name)
{
	Material* mat = new Material();
	AddResourceToMap(a_Name, mat, m_MaterialMap);
	return ObserverPtr<Material>(mat);
}

// Source : https://takinginitiative.wordpress.com/2011/12/11/directx-1011-basic-shader-reflection-automatic-input-layout-creation/
HRESULT Lux::Core::Internal::ResourceHandlerDX11::CreateInputLayoutDescFromVertexShaderSignature( ID3DBlob* pShaderBlob, ID3D11Device* pD3DDevice, ID3D11InputLayout** pInputLayout )
{
	// Reflect shader info
	ID3D11ShaderReflection* pVertexShaderReflection = NULL;
	if ( FAILED( D3DReflect( pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**) &pVertexShaderReflection ) ) )
	{
		return S_FALSE;
	}

	// Get shader info
	D3D11_SHADER_DESC shaderDesc;
	pVertexShaderReflection->GetDesc( &shaderDesc );

	// Read input layout description from shader info
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
	for ( unsigned int i=0; i< shaderDesc.InputParameters; i++ )
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		pVertexShaderReflection->GetInputParameterDesc(i, &paramDesc );

		// fill out input element desc
		D3D11_INPUT_ELEMENT_DESC elementDesc;
		elementDesc.SemanticName = paramDesc.SemanticName;
		elementDesc.SemanticIndex = paramDesc.SemanticIndex;
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;   

		// determine DXGI format
		if ( paramDesc.Mask == 1 )
		{
			if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 ) elementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 ) elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32 ) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if ( paramDesc.Mask <= 3 )
		{
			if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32 ) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if ( paramDesc.Mask <= 7 )
		{
			if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if ( paramDesc.Mask <= 15 )
		{
			if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		//save element desc
		inputLayoutDesc.push_back(elementDesc);
	}       

	// Try to create Input Layout
	HRESULT hr = pD3DDevice->CreateInputLayout( &inputLayoutDesc[0], inputLayoutDesc.size(), pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), pInputLayout );

	//Free allocation shader reflection memory
	pVertexShaderReflection->Release();
	return hr;
}

Lux::Core::ObserverPtr<Lux::Core::Mesh> Lux::Core::Internal::ResourceHandlerDX11::GetMesh(const String& a_Name)
{
	return GetResource(a_Name, m_MeshMap);
}


Lux::Core::ObserverPtr<Lux::Core::Material> Lux::Core::Internal::ResourceHandlerDX11::GetMaterial(const String& a_Name)
{
	return GetResource(a_Name, m_MaterialMap);
}

bool Lux::Core::Internal::ResourceHandlerDX11::MaterialExists(const String& a_Name)
{
	return ResourceExists(a_Name, m_MaterialMap);
}

bool Lux::Core::Internal::ResourceHandlerDX11::MeshExists(const String& a_Name)
{
	return ResourceExists(a_Name, m_MeshMap);
}

bool Lux::Core::Internal::ResourceHandlerDX11::Texture2DExists(const String& a_Name)
{
	return ResourceExists(a_Name, m_Texture2DMap);
}

Lux::Core::ObserverPtr<Lux::Core::Texture2D> Lux::Core::Internal::ResourceHandlerDX11::GetTexture2D(const String& a_Name)
{
	return GetResource(a_Name, m_Texture2DMap);
}

bool Lux::Core::Internal::ResourceHandlerDX11::DeleteTexture2D(const String& a_Name)
{
	return DeleteResource(a_Name, m_Texture2DMap);
}

void Lux::Core::Internal::ResourceHandlerDX11::AddFileNameToMap(const String& a_Str, Mesh* a_Ent)
{
	m_LoadedFilenameMeshes.insert(std::make_pair(Key(a_Str), a_Ent));
}

Lux::Core::Mesh* Lux::Core::Internal::ResourceHandlerDX11::GetLoadedMesh(const String& a_FileStr)
{
	MeshMapSimple::iterator it = m_LoadedFilenameMeshes.find(Key(a_FileStr));

	if (it != m_LoadedFilenameMeshes.end())
	{
		return it->second;
	}

	return nullptr;
}

void Lux::Core::Internal::ResourceHandlerDX11::AddShaderToMap(const String& a_Str, Shader* a_Shader)
{
	AddResourceToMap(a_Str, a_Shader, m_ShaderMap);
}

bool Lux::Core::Internal::ResourceHandlerDX11::ShaderExists(const String& a_Name)
{
	return ResourceExists(a_Name, m_ShaderMap);
}

Lux::Core::ObserverPtr<Lux::Core::Shader> Lux::Core::Internal::ResourceHandlerDX11::GetShader(const String& a_Name)
{
	return GetResource(a_Name, m_ShaderMap);
}

void Lux::Core::Internal::ResourceHandlerDX11::AddInputLayoutToMap(const String& a_Str, ID3D11InputLayout* a_Layout)
{
	m_InputLayouts.insert(std::make_pair(Key(a_Str), Microsoft::WRL::ComPtr<ID3D11InputLayout>(a_Layout)));
}

Lux::Core::ObserverPtr<Lux::Core::Texture1D> Lux::Core::Internal::ResourceHandlerDX11::GetTexture1D(const String& a_Name)
{
	return GetResource(a_Name, m_Texture1DMap);
}

Lux::Core::ObserverPtr<Lux::Core::Texture3D> Lux::Core::Internal::ResourceHandlerDX11::GetTexture3D(const String& a_Name)
{
	return GetResource(a_Name, m_Texture3DMap);
}

bool Lux::Core::Internal::ResourceHandlerDX11::Texture1DExists(const String& a_Name)
{
	return ResourceExists(a_Name, m_Texture1DMap);
}

bool Lux::Core::Internal::ResourceHandlerDX11::Texture3DExists(const String& a_Name)
{
	return ResourceExists(a_Name, m_Texture3DMap);
}

bool Lux::Core::Internal::ResourceHandlerDX11::DeleteTexture1D(const String& a_Name)
{
	return DeleteResource(a_Name, m_Texture1DMap);
}

bool Lux::Core::Internal::ResourceHandlerDX11::DeleteTexture3D(const String& a_Name)
{
	return DeleteResource(a_Name, m_Texture3DMap);
}


Lux::Core::Internal::ResourceHandlerDX11::ResourceHandlerDX11(RenderWindowDX11* a_RenderWindow) :
m_RenderWindow(a_RenderWindow)
{
	LUX_LOG(Utility::logINFO) << "Resource Handler created successfully.";
}

Lux::Core::ObserverPtr<Lux::Core::Texture1D> Lux::Core::Internal::ResourceHandlerDX11::CreateTexture1DFromFile(const String& a_File, const String& a_TexName, const String& a_SamplerName /*= String("LuxDefaultSampler")*/)
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

	Internal::Texture1DDX11* tex1d = new Internal::Texture1DDX11(imgWidth, bits);

	// Put it in the map
	tex1d->SetSampler(GetTextureSampler("LuxDefaultSampler"));
	AddResourceToMap(a_TexName, (Texture1D*)tex1d, m_Texture1DMap);

	//Free FreeImage's copy of the data
	FreeImage_Unload(convertedBitmap);
	Utility::SafePtrDelete(file);
	return ObserverPtr<Texture1D>(tex1d);
}

Lux::Core::ObserverPtr<Lux::Core::Texture1D> Lux::Core::Internal::ResourceHandlerDX11::CreateTexture1DFromMemory(FileInfo* a_Info, const String& a_TexName, const String& a_SamplerName /*= String("LuxDefaultSampler")*/)
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

	Internal::Texture1DDX11* tex1d = new Internal::Texture1DDX11(imgWidth, bits);

	// Put it in the map
	tex1d->SetSampler(GetTextureSampler("LuxDefaultSampler"));
	AddResourceToMap(a_TexName, (Texture1D*)tex1d, m_Texture1DMap);

	//Free FreeImage's copy of the data
	FreeImage_Unload(convertedBitmap);
	Utility::SafePtrDelete(a_Info);
	return ObserverPtr<Texture1D>(tex1d);
}

// TODO - 3D textures
Lux::Core::ObserverPtr<Lux::Core::Texture3D> Lux::Core::Internal::ResourceHandlerDX11::CreateTexture3DFromFile(const String& a_File, const String& a_TexName, const String& a_SamplerName /*= String("LuxDefaultSampler")*/)
{
	return nullptr;
}

Lux::Core::ObserverPtr<Lux::Core::Texture3D> Lux::Core::Internal::ResourceHandlerDX11::CreateTexture3DFromMemory(FileInfo* a_Info, const String& a_TexName, const String& a_SamplerName /*= String("LuxDefaultSampler")*/)
{
	return nullptr;
}

Lux::Core::ObserverPtr<Lux::Core::TextureSampler> Lux::Core::Internal::ResourceHandlerDX11::CreateTextureSampler(const String& a_Name, TextureSamplerOptions& a_InitOptions)
{
	TextureSamplerDX11* sampler = new TextureSamplerDX11(m_RenderWindow->GetDeviceContextPtr(), a_InitOptions);
	AddResourceToMap(a_Name, (TextureSampler*)sampler, m_SamplerMap);
	return ObserverPtr<TextureSampler>(sampler);
}

Lux::Core::ObserverPtr<Lux::Core::TextureSampler> Lux::Core::Internal::ResourceHandlerDX11::GetTextureSampler(const String& a_Name)
{
	return GetResource(a_Name, m_SamplerMap);
}

bool Lux::Core::Internal::ResourceHandlerDX11::DeleteTextureSampler(const String& a_Name)
{
	return DeleteResource(a_Name, m_SamplerMap);
}

bool Lux::Core::Internal::ResourceHandlerDX11::TextureSamplerExists(const String& a_Name)
{
	return ResourceExists(a_Name, m_SamplerMap);
}

Lux::Core::ObserverPtr<Lux::Core::PhysicsMaterial> Lux::Core::Internal::ResourceHandlerDX11::CreatePhysicsMaterial(const String& a_Name, float a_Restitution /*= 0.0f*/, float a_DynamicFriction /*= 0.0f*/, float a_StaticFriction /*= 0.0f*/)
{
	PhysicsMaterial* material = new PhysicsMaterial(a_Restitution, a_DynamicFriction, a_StaticFriction);
	AddResourceToMap(a_Name, material, m_PhysicsMaterialMap);
	return ObserverPtr<PhysicsMaterial>(material);
}

Lux::Core::ObserverPtr<Lux::Core::PhysicsMaterial> Lux::Core::Internal::ResourceHandlerDX11::GetPhysicsMaterial(const String& a_Name)
{
	return GetResource(a_Name, m_PhysicsMaterialMap);
}

bool Lux::Core::Internal::ResourceHandlerDX11::PhysicsMaterialExists(const String& a_Name)
{
	return ResourceExists(a_Name, m_PhysicsMaterialMap);
}

bool Lux::Core::Internal::ResourceHandlerDX11::DeletePhysicsMaterial(const String& a_Name)
{
	return DeleteResource(a_Name, m_PhysicsMaterialMap);
}

void Lux::Core::Internal::ResourceHandlerDX11::AssignLoadedTexturesToSubMesh(SubMesh* a_SubMesh, aiMaterial* a_Mat)
{
	for (unsigned int i = 0; i < LUX_TEXTURES_PER_MESH; i++)
	{
		aiString texName;
		aiReturn texFound = a_Mat->GetTexture((aiTextureType)(i+1), 0, &texName);
		if (texFound == AI_SUCCESS)
		{
			a_SubMesh->SetTexture((TextureIndex)(i), GetResource(String(texName.C_Str()), m_Texture2DMap));
		}
		else
			a_SubMesh->SetTexture((TextureIndex)(i), GetResource(LUX_DEFAULT_TEX, m_Texture2DMap));
	}
}


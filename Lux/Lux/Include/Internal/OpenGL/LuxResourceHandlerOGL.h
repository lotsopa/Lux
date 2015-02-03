#ifndef LUX_RESOURCE_HANDLER_OGL_H
#define LUX_RESOURCE_HANDLER_OGL_H

namespace Lux
{
	namespace Core
	{
		class Material;
		class SubMesh;
		class Mesh;
		class Key;
		class Texture;
		class Texture1D;
		class Texture2D;
		class Texture3D;
		class TextureSampler;
		struct FileInfo;

		namespace Internal
		{
			class ResourceHandlerOGL: public ResourceHandler
			{
			public:
				virtual ~ResourceHandlerOGL();

				Mesh* CreateMeshFromFile(const String& a_File, const String& a_EntityName, unsigned int a_PostProcessFlags);
				Mesh* CreateMeshFromMemory(FileInfo* a_Info, const String& a_EntityName, unsigned int a_PostProcessFlags);
				Texture2D* CreateTexture2DFromFile(const String& a_File, const String& a_TexName);
				Texture2D* CreateTexture2DFromMemory(FileInfo* a_Info, const String& a_TexName);
				Texture1D* CreateTexture1DFromFile(const String& a_File, const String& a_TexName);
				Texture1D* CreateTexture1DFromMemory(FileInfo* a_Info, const String& a_TexName);
				Texture3D* CreateTexture3DFromFile(const String& a_File, const String& a_TexName);
				Texture3D* CreateTexture3DFromMemory(FileInfo* a_Info, const String& a_TexName);
				Shader* CreateShaderFromFile(const String& a_File, const String& a_ShaderName);
				TextureSampler* CreateTextureSampler(const String& a_Name, TextureSamplerOptions& a_InitOptions);

				Material* CreateMaterial(const String& a_Name);
				Mesh* GetMesh(const String& a_Name);
				Material* GetMaterial(const String& a_Name);
				Texture2D* GetTexture2D(const String& a_Name);
				Texture3D* GetTexture3D(const String& a_Name);
				Texture1D* GetTexture1D(const String& a_Name);
				TextureSampler* GetTextureSampler(const String& a_Name);
				bool MaterialExists(const String& a_Name);
				bool MeshExists(const String& a_Name);
				bool TextureSamplerExists(const String& a_Name);
				bool Texture2DExists(const String& a_Name);
				bool Texture1DExists(const String& a_Name);
				bool Texture3DExists(const String& a_Name);
				bool DeleteTexture2D(const String& a_Name);
				bool DeleteTexture1D(const String& a_Name);
				bool DeleteTexture3D(const String& a_Name);
				bool ShaderExists(const String& a_Name);
				Shader* GetShader(const String& a_Name);
				bool DeleteTextureSampler(const String& a_Name);

			protected:
				friend class ResourceHandler;
				ResourceHandlerOGL();
				ResourceHandlerOGL(ResourceHandlerOGL const&);// Don't Implement
				void operator=(ResourceHandlerOGL const&);// Don't implement

				typedef std::map<Key, std::unique_ptr<Texture2D>> Texture2DMap;
				typedef std::map<Key, std::unique_ptr<Texture1D>> Texture1DMap;
				typedef std::map<Key, std::unique_ptr<Texture3D>> Texture3DMap;
				typedef std::map<Key, std::unique_ptr<Mesh>> MeshMap;
				typedef std::map<Key, Mesh*> MeshMapSimple;
				typedef std::map<Key, std::unique_ptr<Material>> MaterialMap;
				typedef std::map<Key, std::unique_ptr<Shader>> ShaderMap;
				typedef std::map<Key, std::unique_ptr<TextureSampler>> SamplerMap;
				MeshMap m_MeshMap;
				MeshMapSimple m_LoadedFilenameMeshes;
				MaterialMap m_MaterialMap;
				Texture2DMap m_Texture2DMap;
				Texture3DMap m_Texture3DMap;
				Texture1DMap m_Texture1DMap;
				ShaderMap m_ShaderMap;
				SamplerMap m_SamplerMap;

#if LUX_THREAD_SAFE == TRUE
				std::mutex m_MeshMapMutex;
				std::mutex m_MaterialMapMutex;
				std::mutex m_TextureMapMutex;
				std::mutex m_ShaderMapMutex;
#endif
				void AddSamplerToMap(const String& a_Str, TextureSampler* a_Sampler);
				void AddMeshToMap(const String& a_Str, Mesh* a_Ent);
				void AddFileNameToMap(const String& a_Str, Mesh* a_Ent);
				Mesh* GetLoadedMesh(const String& a_FileStr);
				void AddMaterialToMap(const String& a_Str, Material* a_Mat);
				void AddTexture2DToMap(const String& a_Str, Texture2D* a_Tex);
				void AddTexture1DToMap(const String& a_Str, Texture1D* a_Tex);
				void AddTexture3DToMap(const String& a_Str, Texture3D* a_Tex);
				void LoadAllTexturesOfTypeFromMaterial(aiMaterial* a_Mat, aiTextureType a_TexType);
				unsigned int LoadOGLShader(GLenum a_ShaderType, FileInfo* a_FileInfo);
				void AddShaderToMap(const String& a_Str, Shader* a_Shader);

				void LoadImageData(const String& a_Path, unsigned int& outWidth, unsigned int& outHeight, unsigned char* outData);
				void LoadImageData(FileInfo* a_File, unsigned int& outWidth, unsigned int& outHeight, unsigned char* outData);
			};
		}
	}
}

#endif
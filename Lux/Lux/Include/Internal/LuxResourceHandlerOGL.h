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
		struct FileInfo;

		namespace Internal
		{
			class ResourceHandlerOGL: public ResourceHandler
			{
			public:
				virtual ~ResourceHandlerOGL();

				Mesh* CreateMeshFromFile(const String& a_File, const String& a_EntityName, unsigned int a_PostProcessFlags);
				Mesh* CreateMeshFromMemory(FileInfo* a_Info, const String& a_EntityName, unsigned int a_PostProcessFlags);
				Texture* CreateTextureFromFile(const String& a_File, const String& a_TexName);


				Mesh* GetMesh(const String& a_Name);
				Material* GetMaterial(const String& a_Name);
				Texture* GetTexture(const String& a_Name);
				bool MaterialExists(const String& a_Name);
				bool MeshExists(const String& a_Name);
				bool TextureExists(const String& a_Name);
				bool DeleteTexture(const String& a_Name);

			protected:
				friend class ResourceHandler;
				ResourceHandlerOGL();
				ResourceHandlerOGL(ResourceHandlerOGL const&);// Don't Implement
				void operator=(ResourceHandlerOGL const&);// Don't implement

				typedef std::map<Key, std::shared_ptr<Texture>> TextureMap;
				typedef std::map<Key, std::shared_ptr<Mesh>> MeshMap;
				typedef std::map<Key, std::shared_ptr<Material>> MaterialMap;
				MeshMap m_MeshMap;
				MeshMap m_LoadedFilenameMeshes;
				MaterialMap m_MaterialMap;
				TextureMap m_TextureMap;

#if LUX_THREAD_SAFE == TRUE
				std::mutex m_MeshMapMutex;
				std::mutex m_MaterialMapMutex;
				std::mutex m_TextureMapMutex;
#endif

				void AddMeshToMap(const String& a_Str, Mesh* a_Ent);
				void AddFileNameToMap(const String& a_Str, Mesh* a_Ent);
				Mesh* GetLoadedMesh(const String& a_FileStr);
				void AddMaterialToMap(const String& a_Str, Material* a_Mat);
				void AddTextureToMap(const String& a_Str, Texture* a_Tex);
				void LoadAllTexturesOfTypeFromMaterial(aiMaterial* a_Mat, aiTextureType a_TexType);
			};
		}
	}
}

#endif
#ifndef LUX_RESOURCE_HANDLER_H
#define LUX_RESOURCE_HANDLER_H

namespace Lux
{
	class Material;
	class SubMesh;
	class Mesh;
	class Key;
	struct FileInfo;

	// TODO - Work on making this class thread safe
	class ResourceHandler
	{
	public:
		static ResourceHandler& GetInstance();
		~ResourceHandler();

		Mesh* CreateEntityFromFile(const String& a_File, const String& a_EntityName, unsigned int a_PostProcessFlags);
		Mesh* CreateEntityFromMemory(FileInfo* a_Info, const String& a_EntityName, unsigned int a_PostProcessFlags);
		const TextureIndex CreateTextureFromFile(const String& a_File, const String& a_TexName);


		Mesh* GetEntity(const String& a_Name);
		Material* GetMaterial(const String& a_Name);
		TextureIndex GetTextureIndex(const String& a_Name);
		bool MaterialExists(const String& a_Name);
		bool EntityExists(const String& a_Name);
		bool TextureExists(const String& a_Name);
		bool DeleteTexture(const String& a_Name);
		bool BindTexture(const String& a_Name);
		void UnbindCurrentTexture();

	private:
		ResourceHandler();
		ResourceHandler(ResourceHandler const&);// Don't Implement
		void operator=(ResourceHandler const&);// Don't implement

		typedef std::map<Key, TextureIndex> TextureMap;
		typedef std::map<Key, Mesh*> EntityMap;
		typedef std::map<Key, Material*> MaterialMap;
		EntityMap m_EntityMap;
		MaterialMap m_MaterialMap;
		TextureMap m_TextureMap;

#if LUX_THREAD_SAFE == TRUE
		std::mutex m_EntityMapMutex;
		std::mutex m_MaterialMapMutex;
		std::mutex m_TextureMapMutex;
#endif

		void AddEntityToMap(const String& a_Str, Mesh* a_Ent);
		void AddMaterialToMap(const String& a_Str, Material* a_Mat);
		void AddTextureToMap(const String& a_Str, TextureIndex a_Idx);
		void LoadAllTexturesOfTypeFromMaterial(aiMaterial* a_Mat, aiTextureType a_TexType);
	};
}

#endif
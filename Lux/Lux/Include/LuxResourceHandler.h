#ifndef LUX_RESOURCE_HANDLER_H
#define LUX_RESOURCE_HANDLER_H

namespace Lux
{
	class Material;
	class Mesh;
	class Entity;
	class Key;
	struct FileInfo;

	// TODO - Work on making this class thread safe
	class ResourceHandler
	{
	public:
		static ResourceHandler& GetInstance();
		~ResourceHandler();

		// TODO - Animations and textures import
		Entity* CreateEntityFromFile(const String a_File, const String a_EntityName, unsigned int a_PostProcessFlags);
		Entity* CreateEntityFromMemory(FileInfo* a_Info, const String a_EntityName, unsigned int a_PostProcessFlags);

		Entity* GetEntity(const String a_Name);
		Material* GetMaterial(const String a_Name);
		bool MaterialExists(const String a_Name);
		bool EntityExists(const String a_Name);

	private:
		ResourceHandler();
		ResourceHandler(ResourceHandler const&);// Don't Implement
		void operator=(ResourceHandler const&);// Don't implement

		typedef std::map<Key, Entity*> EntityMap;
		typedef std::map<Key, Material*> MaterialMap;
		EntityMap m_EntityMap;
		MaterialMap m_MaterialMap;

#if LUX_THREAD_SAFE == TRUE
		std::mutex m_EntityMapMutex;
		std::mutex m_MaterialMapMutex;
#endif

		void AddEntityToMap(const String& a_Str, Entity* a_Ent);
		void AddMaterialToMap(const String& a_Str, Material* a_Mat);
	};
}

#endif
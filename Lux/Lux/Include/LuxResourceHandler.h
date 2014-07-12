#ifndef LUX_RESOURCE_HANDLER_H
#define LUX_RESOURCE_HANDLER_H

namespace Lux
{
	class Mesh;
	class Entity;
	class Key;
	struct FileInfo;
	class ResourceHandler
	{
	public:
		static ResourceHandler& GetInstance();
		~ResourceHandler();

		// TODO
		Entity* CreateEntityFromFile(const String a_File, const String a_MeshName, unsigned int a_PostProcessFlags);
		Entity* CreateEntityFromMemory(FileInfo* a_Info, const String a_MeshName, unsigned int a_PostProcessFlags);

	private:
		ResourceHandler();
		ResourceHandler(ResourceHandler const&);// Don't Implement
		void operator=(ResourceHandler const&);// Don't implement

		std::map<Key, Entity> m_EntityMap;
		std::mutex m_EntityMapMutex;
	};
}

#endif
#include "LuxPCH.h"
#include "LuxFileSystem.h"

Lux::FileSystem& Lux::FileSystem::GetInstance()
{
	static FileSystem instance;
	return instance;
}

Lux::FileSystem::~FileSystem()
{
	if (PHYSFS_isInit())
	{
		int res = PHYSFS_deinit();

		if (!res)
		{
			LUX_LOG(logERROR) << "Error destroying File System : " << PHYSFS_getLastError();
			MessageBox(nullptr, "Could not properly destroy File System class.", "Error", MB_ICONERROR | MB_SETFOREGROUND);
			throw std::logic_error(PHYSFS_getLastError());
		}
	}
}

Lux::FileSystem::FileSystem()
{
	int res = PHYSFS_init(nullptr);

	if (!res)
	{
		LUX_LOG(logERROR) << "Error initializing File System : " << PHYSFS_getLastError();
		MessageBox(nullptr, "Could not initialize File System class.", "Error", MB_ICONERROR | MB_SETFOREGROUND);
		throw std::logic_error(PHYSFS_getLastError());
	}
}

bool Lux::FileSystem::AddResourcePath(const String a_Path)
{
	int res = PHYSFS_addToSearchPath(a_Path.c_str(), 0);

	if (!res)
	{
		LUX_LOG(logWARNING) << "Directory: " << a_Path << " was not added to the search path. " << PHYSFS_getLastError();
		return false;
	}

	return true;
}

bool Lux::FileSystem::RemoveResourcePath(const String a_Path)
{
	int res = PHYSFS_removeFromSearchPath(a_Path.c_str());

	if (!res)
	{
		LUX_LOG(logWARNING) << "Directory " << a_Path << " was not removed from the search path. " << PHYSFS_getLastError();
		return false;
	}

	return true;
}

bool Lux::FileSystem::ResourcePathExists(const String a_Path)
{
	char** path = PHYSFS_getSearchPath();

	if (path == nullptr)
	{
		LUX_LOG(logERROR) << "Error fetching the search path. " << PHYSFS_getLastError();
		MessageBox(nullptr, "Could not get the File System search path.", "Error", MB_ICONERROR | MB_SETFOREGROUND);
		throw std::logic_error(PHYSFS_getLastError());
	}

	// Search for the string inside the current search path
	bool retval = false;
	for (char** i = path; i != nullptr; i++)
	{
		if (a_Path.compare(*i) == 0)
		{
			retval = true;
			break;
		}
	}
	PHYSFS_freeList(path); // Free the list
	return retval;
}

bool Lux::FileSystem::FileExists(const String a_Path)
{
	int res = PHYSFS_exists(a_Path.c_str());

	if (!res)
	{
		return false;
	}

	return true;
}

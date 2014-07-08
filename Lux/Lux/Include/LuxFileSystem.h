#ifndef LUX_FILE_SYSTEM_H
#define LUX_FILE_SYSTEM_H

namespace Lux
{
	struct FileInfo
	{
		char* m_RawData;
		unsigned int m_Length;
	};

	class FileSystem
	{
	public:
		static FileSystem& GetInstance();
		~FileSystem();

		// Resource path functions
		bool AddResourcePath(const String a_Path);
		bool RemoveResourcePath(const String a_Path);
		bool ResourcePathExists(const String a_Path);

		// File functions

		// Returns if a file exists in any of the resource paths
		bool FileExists(const String a_Path);

		
		// TODO - The rest

	private:
		FileSystem();
		FileSystem(FileSystem const&);// Don't Implement
		void operator=(FileSystem const&);// Don't implement
	};
}

#endif
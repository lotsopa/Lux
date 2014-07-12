#ifndef LUX_FILE_HANDLER_H
#define LUX_FILE_HANDLER_H

namespace Lux
{
	// Helper struct that encapsulates the raw file info that is loaded in memory
	struct FileInfo
	{
		FileInfo() : m_RawData(nullptr), m_DataLength(0)
		{

		}

		FileInfo(char* a_Data, unsigned int a_DataLength) : m_RawData(a_Data), m_DataLength(a_DataLength)
		{

		}

		FileInfo(unsigned int a_Length) : m_DataLength(a_Length)
		{
			m_RawData = new char[sizeof(char) * m_DataLength];
		}

		~FileInfo()
		{
			SafeArrayDelete(m_RawData);
		}

		char* m_RawData;
		unsigned int m_DataLength;
	};

	/* A struct that represents a currently opened file.
	 It only holds an opaque pointer, so no manipulations should be done to it.
	 It's use to the user is just to be passed around to the different API functions*/
	typedef PHYSFS_File OpenedFile;

	class FileHandler
	{
	public:
		static FileHandler& GetInstance();
		~FileHandler();

		// Resource path functions
		bool AddResourcePath(const String a_Path);
		bool RemoveResourcePath(const String a_Path);
		bool ResourcePathExists(const String a_Path);

		// File functions

		enum FileOpenMode
		{
			FILE_OPEN_READ,
			FILE_OPEN_WRITE,
			FILE_OPEN_APPEND
		};

		// Returns if a file exists in any of the resource paths
		bool FileExists(const String a_Path);

		// Opens the specified file in the specified Open mode. Throws error if file doesn't exist.
		OpenedFile* OpenFile(const String a_File, FileOpenMode a_OpenMode);

		// Loads the specified file in memory and returns the file info. Throws if file doesn't exist.
		// The caller must take ownership of the returned pointer and is responsible for managing its lifetime.
		FileInfo* LoadFileInMemory(const String a_Path);

		// Gets the file length of an opened file. Throws if the length cannot be read.
		int64 GetFileLength(OpenedFile* a_File);

		// Load the file data from the specified already opened file into memory. Throws if the data read is invalid.
		void LoadFileDataInto(OpenedFile* a_File, FileInfo* a_Dest);

		// Closes an opened file object and invalidates the pointer to it. Does nothing if called with nullptr.
		void CloseOpenedFile(OpenedFile* a_File);

		// Deletes an already loaded file info and invalidates the pointer which is passed to it. Does nothing if passed pointer is null.
		void DeleteLoadedFileInfo(FileInfo* a_FileInfo);

	private:
		FileHandler();
		FileHandler(FileHandler const&);// Don't Implement
		void operator=(FileHandler const&);// Don't implement
	};
}

#endif
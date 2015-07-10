#ifndef LUX_MEMORY_POOL_H
#define LUX_MEMORY_POOL_H

namespace Lux
{
	namespace Core
	{
		// TODO - Thread safety

		/*
		A memory pool is a data structure which holds a big chunk of memory and creates/deletes objects inside that memory.
		This is much faster than calling new/delete for every small object that needs to be created.
		The current implementation needs to know what kind of objects it will hold.
		The memory pool also helps tremendously with reducing memory fragmentation.
		It makes sure the memory allocated is sequential so it can provide better cache coherency.
		The memory pool class can be used with ANY class type.
		*/

		template<class ObjectType>
		class ObjectHandle
		{
		public:

			ObjectHandle() : m_RawPtr(nullptr), m_Index(0)
			{

			}

			ObjectHandle(ObjectType* a_RawPtr, unsigned int a_Idx) : m_RawPtr(a_RawPtr), m_Index(a_Idx)
			{

			}

			~ObjectHandle()
			{

			}

			__forceinline ObjectType* const GetRawPtr() const
			{
				return m_RawPtr;
			}

			__forceinline const bool IsValid() const
			{
				return m_RawPtr != nullptr;
			}

			bool operator<(const ObjectHandle& handle1) const { return m_Index < handle1.m_Index; }

		private:
			ObjectType* m_RawPtr;
			unsigned int m_Index;
			template <class T>
			friend class ObjectPool;

			friend class SceneManager;
		};

		template<class ObjectType>
		class ObjectPool
		{
		public:

			struct ObjectEntry
			{
				ObjectEntry() : m_Next(nullptr), m_Index(0) { };
				~ObjectEntry() {};
				unsigned int m_Index;
				ObjectEntry* m_Next;
				ObjectType m_Object;
			};
			ObjectPool(const unsigned int a_InitialSize, const unsigned int a_GrowSize) :
				m_MaxObjects(a_InitialSize), m_GrowSize(a_GrowSize)
			{
				AllocateObjects();
			}

			~ObjectPool()
			{
				FreeObjects();
				m_ObjectHandleMap.clear();
			}

			inline const unsigned int GetMaximumObjects() { return m_MaxObjects; };

			void Reallocate(const unsigned int a_MaxObjects)
			{
				FreeObjects();
				m_MaxObjects = a_MaxObjects;
				AllocateObjects();
			}

			// Creates an object and returns the pointer to the created object.
			// Returns null if an object cannot be created.
			ObjectHandle<ObjectType>& CreateObject()
			{
				if (m_FirstAvailable->m_Next == nullptr)
				{
					// We reached the end, need to grow the pool
					Grow();
				}

				// Remove it from the available list.
				ObjectEntry* newObject = m_FirstAvailable;
				m_FirstAvailable = newObject->m_Next;
				unsigned int idx = newObject->m_Index;
				m_ObjectHandleMap[idx].m_RawPtr = &newObject->m_Object;
				return m_ObjectHandleMap[idx];
			}

			// Deletes an object and invalidates the provided pointer. 
			// Returns true on success and false on failure. 
			//The passed pointer is set to null regardless.
			bool  DeleteObject(ObjectHandle<ObjectType>& a_Obj)
			{
				// Add this object to the front of the list.
				unsigned int idx = a_Obj.m_Index;
				m_ObjectMemory[idx].m_Next = m_FirstAvailable;
				m_FirstAvailable = &m_ObjectMemory[idx];

				// Invalidate handle
				a_Obj.m_RawPtr = nullptr;
				return true;
			}

		private:

			unsigned int m_MaxObjects;
			unsigned int m_GrowSize;
			ObjectEntry* m_ObjectMemory;
			ObjectEntry* m_FirstAvailable;
			std::unordered_map<unsigned int, ObjectHandle<ObjectType>> m_ObjectHandleMap;

			void Grow()
			{
				Reallocate(m_MaxObjects + m_GrowSize);
			}

			void FreeObjects()
			{
				Utility::SafeArrayDelete(m_ObjectMemory);
				m_ObjectHandleMap.clear();
			}

			void AllocateObjects()
			{
				// Allocate
				m_ObjectMemory = new ObjectEntry[m_MaxObjects];

				// Set first available
				m_FirstAvailable = &m_ObjectMemory[0];

				// Set the rest
				for (unsigned int i = 0; i < m_MaxObjects-1; i++)
				{
					m_ObjectMemory[i].m_Index = i;
					m_ObjectMemory[i].m_Next = &m_ObjectMemory[i + 1];

					// Init the handle map
					m_ObjectHandleMap[i].m_RawPtr = &m_ObjectMemory[i].m_Object;
					m_ObjectHandleMap[i].m_Index = i;
				}

				// Set last element's next pointer to null
				m_ObjectMemory[m_MaxObjects - 1].m_Index = m_MaxObjects - 1;
				m_ObjectMemory[m_MaxObjects - 1].m_Next = nullptr;

				// Handle map
				m_ObjectHandleMap[m_MaxObjects - 1].m_RawPtr = &m_ObjectMemory[m_MaxObjects - 1].m_Object;
				m_ObjectHandleMap[m_MaxObjects - 1].m_Index = m_MaxObjects - 1;
			}
		};
	}

};

#endif
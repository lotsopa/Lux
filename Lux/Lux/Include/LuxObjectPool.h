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
		The only prerequisite for the class which you plan to use the memory pool with is to have a an Index field.
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
				if (m_AvailableObjects.empty())
				{
					Grow();
				}

				unsigned int idx = m_AvailableObjects.front();
				m_AvailableObjects.pop();
				m_ObjectHandleMap[idx].m_RawPtr = &m_ObjectMemory[idx];
				return m_ObjectHandleMap[idx];
			}

			// Deletes an object and invalidates the provided pointer. 
			// Returns true on success and false on failure. 
			//The passed pointer is set to null regardless.
			bool  DeleteObject(ObjectHandle<ObjectType>& a_Obj)
			{
				m_AvailableObjects.push(a_Obj.m_Index);
				a_Obj.m_RawPtr = nullptr;
				return true;
			}

		private:

			unsigned int m_MaxObjects;
			unsigned int m_GrowSize;
			std::vector<ObjectType> m_ObjectMemory;
			std::queue<unsigned int> m_AvailableObjects;
			std::map<unsigned int, ObjectHandle<ObjectType>> m_ObjectHandleMap;

			void Grow()
			{
				unsigned int newVal = m_MaxObjects + m_GrowSize;

				m_ObjectMemory.reserve(newVal);
				for (unsigned int i = m_MaxObjects; i < newVal; i++)
				{
					m_ObjectMemory.push_back(ObjectType());
					m_ObjectMemory[i].m_Index = i;
					m_AvailableObjects.push(i);
				}
				m_MaxObjects = newVal;

				for (unsigned int i = 0; i < m_MaxObjects; i++)
				{
					m_ObjectHandleMap[i].m_RawPtr = &m_ObjectMemory[i];
					m_ObjectHandleMap[i].m_Index = i;
				}
			}

			void FreeObjects()
			{
				/*for (unsigned int i = 0; i < m_MaxObjects; i++)
				{
					Utility::SafePtrDelete(m_ObjectMemory[i]);
				}*/
				std::queue<unsigned int> empty;
				m_AvailableObjects.swap(empty);
				std::vector<ObjectType> emptyVec;
				m_ObjectMemory.swap(emptyVec);

				for (unsigned int i = 0; i < m_MaxObjects; i++)
				{
					m_ObjectHandleMap[i].m_RawPtr = nullptr;
				}
			}

			void AllocateObjects()
			{
				m_ObjectMemory.reserve(m_MaxObjects);
				for (unsigned int i = 0; i < m_MaxObjects; i++)
				{
					m_ObjectMemory.push_back(ObjectType());
					m_ObjectMemory[i].m_Index = i;
					m_AvailableObjects.push(i);
					m_ObjectHandleMap[i].m_RawPtr = &m_ObjectMemory[i];
					m_ObjectHandleMap[i].m_Index = i;
				}
			}
		};
	}

};

#endif
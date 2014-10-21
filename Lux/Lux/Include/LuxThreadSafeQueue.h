#ifndef LUX_THREAD_SAFE_QUEUE_H
#define LUX_THREAD_SAFE_QUEUE_H

namespace Lux
{
	namespace Utility
	{
		template<class T>
		class ThreadSafeObjectQueue
		{
		public:

			/*
			Pops an item from the end of the queue and returns it.
			If the queue is empty, throws exception.
			*/
			T Pop()
			{
				std::unique_lock<std::mutex> lock(m_Mutex);

				if (m_Queue.empty())
				{
					Utility::ThrowError("Trying to receive an item from an empty queue.");
					return T();
				}

				T retItem = m_Queue.front();
				m_Queue.pop();
				return retItem;
			}

			/*
			Pushes an item in the queue.
			*/
			void Push(const T& a_Item)
			{
				std::unique_lock<std::mutex> lock(m_Mutex);
				m_Queue.push(a_Item);
			}

			/*
			Checks if the queue is empty.
			Returns true if the queue is empty, false otherwise.
			*/
			const bool Empty()
			{
				std::unique_lock<std::mutex> lock(m_Mutex);

				return m_Queue.empty();
			}

		private:
			std::queue<T> m_Queue;
			std::mutex m_Mutex;
		};

		template<class T>
		class ThreadSafePtrQueue
		{
		public:

			/*
			Pops an item from the end of the queue and returns it.
			If the queue is empty, returns NULL.
			*/
			T* Pop()
			{
				std::unique_lock<std::mutex> lock(m_Mutex);

				if (m_Queue.empty())
				{
					return nullptr;
				}

				T* retItem = m_Queue.front();
				m_Queue.pop();
				return retItem;
			}

			/*
			Pushes an item in the queue.
			The function throws if the pushed item is NULL.
			*/
			void Push(T* a_Item)
			{
				if (a_Item == nullptr)
				{
					Utility::ThrowError("NULL item may not be pushed into a queue.");
				}
				std::unique_lock<std::mutex> lock(m_Mutex);
				m_Queue.push(a_Item);
			}

			/*
			Checks if the queue is empty.
			Returns true if the queue is empty, false otherwise.
			*/
			const bool Empty()
			{
				std::unique_lock<std::mutex> lock(m_Mutex);

				return m_Queue.empty();
			}

		private:
			std::queue<T*> m_Queue;
			std::mutex m_Mutex;
		};
	}
}

#endif
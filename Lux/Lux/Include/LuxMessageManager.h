#ifndef LUX_MESSAGE_MANAGER_H
#define LUX_MESSAGE_MANAGER_H
#include "LuxKey.h"

namespace Lux
{
	namespace Core
	{
		/*
		Message Callback object, which wraps an std::function.
		Used to register callbacks with the MessageManager.
		*/
		class MessageCallback
		{
		public:
			MessageCallback() {};
			~MessageCallback() {};
			typedef std::function <void(void*)> CallbackFunc;

			template<class Callee>
			void Init(const CallbackFunc& a_CallbackFunc)
			{
				m_ObjName.SetName(typeid(Callee).name());
				m_CallbackFunc = a_CallbackFunc;
			}

			inline bool operator==(const MessageCallback& a_Other) const
			{
				return m_ObjName == a_Other.m_ObjName;
			}

			inline bool operator != (const MessageCallback& a_Other) const 
			{
				return m_ObjName != a_Other.m_ObjName;
			}

			inline bool IsInit() { return !m_ObjName.IsEmpty(); }

		private:
			Key m_ObjName;
			CallbackFunc m_CallbackFunc;
			friend class MessageManager;
		};

		/*
		The Message manager provides an API to register callbacks for certain messages
		and call the registered callbacks when a message is sent.
		The only limitation of the system is that a class type can only have one callback per message type.
		*/
		class MessageManager
		{
		public:
			MessageManager();
			~MessageManager();

			/*
			Registers a message callback for a certain ID.
			Returns true if registration was successful, false otherwise.
			*/
			bool RegisterMsgCallback(unsigned int a_MsgType, MessageCallback& a_Callback);

			/*
			De-registers an already registered callback for a certain ID.
			Return true on success, false otherwise.
			*/
			bool DeregisterMsgCallback(unsigned int a_MsgType, MessageCallback& a_Callback);

			/*
			Checks if a particular callback si registered for a specific ID.
			Returns true if a particular callback is registered for a message ID, false otherwise.
			*/
			bool MsgCallbackRegistered(unsigned int a_MsgType, MessageCallback& a_Callback);

			/*
			Sends a message to all registered callbacks.
			This function will return when all callbacks have been executed.
			*/
			void BroadcastMessage(unsigned int a_Msg, void* a_UserData);

		private:
			
			typedef std::multimap <unsigned int, MessageCallback> MessageMap;
			MessageMap m_MessageMap;
		};
	}
}

#endif
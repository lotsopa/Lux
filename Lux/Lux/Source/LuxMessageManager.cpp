#include "LuxPCH.h"
#include "LuxMessageManager.h"

Lux::Core::MessageManager::MessageManager()
{

}

Lux::Core::MessageManager::~MessageManager()
{

}

bool Lux::Core::MessageManager::RegisterMsgCallback(unsigned int a_MsgType, MessageCallback& a_Callback)
{
	if (!a_Callback.IsInit())
		return false;

	if (!MsgCallbackRegistered(a_MsgType, a_Callback))
	{
		m_MessageMap.insert(std::make_pair(a_MsgType, a_Callback));
		return true;
	}
	else
	{
		return false;
	}
		
}

bool Lux::Core::MessageManager::DeregisterMsgCallback(unsigned int a_MsgType, MessageCallback& a_Callback)
{
	MessageMap::iterator it1 = m_MessageMap.lower_bound(a_MsgType);
	MessageMap::iterator it2 = m_MessageMap.upper_bound(a_MsgType);

	while (it1 != it2)
	{
		if (a_Callback == it1->second)
		{
			m_MessageMap.erase(it1);
			return true;
		}
		++it1;
	}
	return false;
}

bool Lux::Core::MessageManager::MsgCallbackRegistered(unsigned int a_MsgType, MessageCallback& a_Callback)
{
	MessageMap::iterator it1 = m_MessageMap.lower_bound(a_MsgType);
	MessageMap::iterator it2 = m_MessageMap.upper_bound(a_MsgType);

	while (it1 != it2)
	{
		if (a_Callback == it1->second)
		{
			return true;
		}
		++it1;
	}
	return false;
}

void Lux::Core::MessageManager::BroadcastMessage(unsigned int a_Msg, void* a_UserData)
{
	MessageMap::iterator it1 = m_MessageMap.lower_bound(a_Msg);
	MessageMap::iterator it2 = m_MessageMap.upper_bound(a_Msg);

	while (it1 != it2)
	{
		// Execute function
		it1->second.m_CallbackFunc(a_UserData);
		++it1;
	}
}

#include "LuxPCH.h"
#include "LuxKey.h"

Lux::Key::StringHash Lux::Key::m_StringHasher;

Lux::Key::Key() : m_Name()
{
	m_StringHasher(m_Name);
}

Lux::Key::Key(const String& a_Name) : m_Name(a_Name)
{
	m_HashedValue = m_StringHasher(m_Name);
}

Lux::Key::Key(const char* a_Name) : m_Name(a_Name)
{
	m_HashedValue = m_StringHasher(m_Name);
}

Lux::Key::~Key()
{

}

const std::string& Lux::Key::GetName() const
{
	return m_Name;
}

void Lux::Key::SetName(const String& a_Name)
{
	m_Name = a_Name;
	m_HashedValue = m_StringHasher(m_Name);
}

const size_t Lux::Key::GetHashedValue() const
{
	return m_HashedValue;
}

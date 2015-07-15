#include "LuxPCH.h"
#include "LuxKey.h"

Lux::Core::Key::StringHash Lux::Core::Key::m_StringHasher;

Lux::Core::Key::Key() : m_Name()
{
	m_StringHasher(m_Name);
}

Lux::Core::Key::Key(const String& a_Name) : m_Name(a_Name)
{
	m_HashedValue = m_StringHasher(m_Name);
}

Lux::Core::Key::Key(const char* a_Name) : m_Name(a_Name)
{
	m_HashedValue = m_StringHasher(m_Name);
}

Lux::Core::Key::~Key()
{

}

const std::string& Lux::Core::Key::GetName() const
{
	return m_Name;
}

void Lux::Core::Key::SetName(const String& a_Name)
{
	m_Name = a_Name;
	m_HashedValue = m_StringHasher(m_Name);
}

const size_t Lux::Core::Key::GetHashedValue() const
{
	return m_HashedValue;
}

const bool Lux::Core::Key::IsEmpty() const
{
	return m_Name.empty();
}

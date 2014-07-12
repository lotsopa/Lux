#ifndef LUX_KEY_H
#define LUX_KEY_H

namespace Lux
{
	class Key
	{
	public:
		Key();
		Key(const String& a_Name);
		~Key();

		const std::string& GetName() const;
		void SetName(const String& a_Name);
		const size_t GetHashedValue() const;

		bool operator<(const Key& key1) const { return m_HashedValue < key1.m_HashedValue; }

		Key operator=(const Key &k)
		{
			m_Name = k.m_Name;
			m_HashedValue = k.m_HashedValue;
			return *this;
		}

		Key operator=(const String &str)
		{
			m_Name = str;
			m_HashedValue = m_StringHasher(m_Name);
			return *this;
		}

		bool operator==(const Key &k) const
		{
			return m_HashedValue == k.m_HashedValue;
		}

		bool operator!=(const Key &k) const
		{
			return m_HashedValue != k.m_HashedValue;
		}

		Key operator+(const Key& k)
		{
			return Key(m_Name + k.m_Name);
		}
	private:
		typedef std::hash<String> StringHash;
		String m_Name;
		static StringHash m_StringHasher;
		size_t m_HashedValue;
	};
}

#endif
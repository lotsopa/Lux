#ifndef LUX_DEFINES_H
#define LUX_DEFINES_H

#define LuxAssert(a) assert(a)

namespace Lux
{
	using namespace glm;
	typedef std::string String;
	typedef long long int64;

	template<typename T>
	static void SafePtrDelete(T* a_Ptr)
	{
		if (a_Ptr)
		{
			delete a_Ptr;
			a_Ptr = nullptr;
		}
	}

	static void ThrowError(const String a_String)
	{
		LUX_LOG(logERROR) << a_String;
		MessageBox(nullptr, a_String.c_str(), "Error", MB_ICONERROR | MB_SETFOREGROUND);
		throw std::logic_error(a_String);
	}
}

#endif
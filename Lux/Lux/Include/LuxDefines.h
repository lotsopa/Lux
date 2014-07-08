#ifndef LUX_DEFINES_H
#define LUX_DEFINES_H

#define LuxAssert(a) assert(a)

namespace Lux
{
	template<typename T>
	void SafePtrDelete(T* a_Ptr)
	{
		if (a_Ptr)
		{
			delete a_Ptr;
			a_Ptr = nullptr;
		}
	}

	using namespace glm;
	typedef std::string String;
}

#endif
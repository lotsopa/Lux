#ifndef LUX_DEFINES_H
#define LUX_DEFINES_H

#define LuxAssert(a) assert(a)

#define LUX_THREAD_SAFE FALSE
#define LUX_LOGGING_ENABLED TRUE
#define LUX_ENTITY_MEMORY_POOL_INITIAL_SIZE 100
#define LUX_ENTITY_MEMORY_POOL_GROW_AMOUNT 20
#define LUX_COMPONENT_MEMORY_POOL_INITIAL_SIZE 100
#define LUX_COMPONENT_MEMORY_POOL_GROW_AMOUNT 20

#define BUFFER_OFFSET(offset) ((void*)(offset))
#define MEMBER_OFFSET(s,m) ((char*)NULL + (offsetof(s,m)))
#define ID_TO_STRING(a) #a

namespace Lux
{
	using namespace glm;

	// Some typedefs
	typedef std::string String;
	typedef long long int64;
	typedef unsigned int TextureIndex;

	const vec4 WINDOW_CLEAR_COLOR(0.2f);
}

#endif
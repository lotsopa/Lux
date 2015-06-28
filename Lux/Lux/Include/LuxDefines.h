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

// Physics defines
#define LUX_WORLD_GRAVITY vec3(0.0f, -9.81f, 0.0f)
#define LUX_PHYSICS_TIMESTEP_SEC 0.01f
#define LUX_NUM_PHYSICS_THREADS 0

#ifdef WIN32
#define LUX_ALIGNED_MALLOC(size, alignment) _aligned_malloc(size, alignment)
#define LUX_ALIGNED_FREE(memoryPtr) _aligned_free(memoryPtr)
#else
#define LUX_ALIGNED_MALLOC(size, alignment) aligned_alloc(alignment, size)
#endif
namespace Lux
{
	using namespace glm;
	using namespace physx;

	// Some typedefs
	typedef std::string String;
	typedef std::wstring WideString;
	typedef long long int64;
	typedef unsigned int TextureIndex; 

	const vec4 WINDOW_CLEAR_COLOR(0.2f);

	enum ShaderProgram
	{
		VERTEX_PROGRAM = 0,
		FRAGMENT_PROGRAM,
		GEOMETRY_PROGRAM,
		NUM_SHADER_PROGRAMS
	};
}

#endif


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)

// System includes
#include <windows.h>
// Windows Runtime Template Library
#include <wrl.h>

// DirectX includes
#include <d3d11_1.h>
#include <d3dcompiler.h>

// Link library dependencies
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dxguid.lib")

#endif

// Standard headers
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <mutex>
#include <assert.h>
#include <queue>
#include <typeinfo>
#include <memory>

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

// OpenGL window management and extension wrangler
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Math library
#define GLM_FORCE_RADIANS
#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/ext.hpp>

// File system library
#include <physfs.h>

// Image loading library
#include <FreeImage.h>

// Assimp
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

// PhysX
#include "PxPhysicsAPI.h"
#include "extensions\PxDefaultSimulationFilterShader.h"

// Engine specific include
#include "LuxDefines.h"
#include "LuxLog.h"
#include "LuxHelpers.h"

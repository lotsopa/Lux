
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)

#include <windows.h>

#endif

// Standard headers
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <assert.h>

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

// Engine specific include
#include "LuxDefines.h"
#include "LuxLog.h"
#include "LuxHelpers.h"

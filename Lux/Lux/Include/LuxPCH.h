
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
#include <assert.h>

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

// Engine specific include
#include "LuxLog.h"
#include "LuxDefines.h"

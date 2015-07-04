#ifndef LUX_LIGHT_SHADER_RESOURCE_H
#define LUX_LIGHT_SHADER_RESOURCE_H
#include "LuxComponent.h"
#include "LuxLight.h"

namespace Lux
{
	namespace Graphics
	{
		LUX_ALIGN_16_BYTES struct LightShaderResource
		{
			LightShaderResource();
			~LightShaderResource();

			vec4 m_Color;
			vec3 m_Direction;
			float m_Intensity;
			vec3 m_Position;
		};
	}
}

#endif
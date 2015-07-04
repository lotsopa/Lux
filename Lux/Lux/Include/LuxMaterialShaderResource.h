#ifndef LUX_MATERIAL_SHADER_RESOURCE_H
#define LUX_MATERIAL_SHADER_RESOURCE_H
#include "LuxMaterialResource.h"

namespace Lux
{
	namespace Graphics
	{
		LUX_ALIGN_16_BYTES struct MaterialShaderResource
		{
			MaterialShaderResource();
			MaterialShaderResource(const Core::MaterialResource& a_MatRes);
			~MaterialShaderResource();
			vec4 m_DiffuseColor;
			vec4 m_SpecularColor;
			vec4 m_EmissiveColor;
			vec4 m_AmbientColor;
			float m_Specular;
		};
	}
}

#endif
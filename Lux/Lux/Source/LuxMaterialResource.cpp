#include "LuxPCH.h"
#include "LuxMaterialResource.h"


Lux::Core::MaterialResource::MaterialResource() :
m_Opacity(1.0f),
m_Reflectivity(0.0f),
m_Shininess(0.0f),
m_ShininessStrength(0.0f),
m_RefractionIndex(0.0f),
m_DiffuseColor(1.0f, 1.0f, 1.0f, 1.0f),
m_AmbientColor(0.1f, 0.1f, 0.1f, 1.0f),
m_EmissiveColor(0.0f, 0.0f, 0.0f, 1.0f),
m_SpecularColor(1.0f, 1.0f, 1.0f, 1.0f),
m_TransparentColor(0.0f, 0.0f, 0.0f, 0.0f)
{

}

Lux::Core::MaterialResource::MaterialResource(aiMaterial& a_Mat)
{
	aiString str;
	aiReturn retval;

	// Name
	retval = a_Mat.Get(AI_MATKEY_NAME, str);
	if (retval != aiReturn_SUCCESS)
	{
		LUX_LOG(Utility::logWARNING) << "Failed to retrieve the material name from file. The field will have its default value.";
	}
	else
	{
		m_Name = str.C_Str();
	}

	// Diffuse
	aiColor4D col;
	retval = a_Mat.Get(AI_MATKEY_COLOR_DIFFUSE, col);
	if (retval != aiReturn_SUCCESS)
	{
		LUX_LOG(Utility::logWARNING) << "Failed to retrieve the diffuse color from file. The field will have its default value.";
	}
	else
	{
		m_DiffuseColor = Utility::ConvertVec4Assimp(col);
	}

	//Specular
	retval = a_Mat.Get(AI_MATKEY_COLOR_SPECULAR, col);
	if (retval != aiReturn_SUCCESS)
	{
		LUX_LOG(Utility::logWARNING) << "Failed to retrieve the specular color from file. The field will have its default value.";
	}
	else
	{
		m_SpecularColor = Utility::ConvertVec4Assimp(col);
	}

	// Emissive
	retval = a_Mat.Get(AI_MATKEY_COLOR_EMISSIVE, col);
	if (retval != aiReturn_SUCCESS)
	{
		LUX_LOG(Utility::logWARNING) << "Failed to retrieve the emissive color from file. The field will have its default value.";
	}
	else
	{
		m_EmissiveColor = Utility::ConvertVec4Assimp(col);
	}

	// Reflective
	retval = a_Mat.Get(AI_MATKEY_COLOR_REFLECTIVE, col);
	if (retval != aiReturn_SUCCESS)
	{
		LUX_LOG(Utility::logWARNING) << "Failed to retrieve the reflective color from file. The field will have its default value.";
	}
	else
	{
		m_ReflectiveColor = Utility::ConvertVec4Assimp(col);
	}

	// Ambient
	retval = a_Mat.Get(AI_MATKEY_COLOR_AMBIENT, col);
	if (retval != aiReturn_SUCCESS)
	{
		LUX_LOG(Utility::logWARNING) << "Failed to retrieve the ambient color from file. The field will have its default value.";
	}
	else
	{
		m_AmbientColor = Utility::ConvertVec4Assimp(col);
	}

	// Transparent
	retval = a_Mat.Get(AI_MATKEY_COLOR_TRANSPARENT, col);
	if (retval != aiReturn_SUCCESS)
	{
		LUX_LOG(Utility::logWARNING) << "Failed to retrieve the transparent color from file. The field will have its default value.";
	}
	else
	{
		m_AmbientColor = Utility::ConvertVec4Assimp(col);
	}

	// Shininess
	retval = a_Mat.Get(AI_MATKEY_SHININESS, m_Shininess);
	if (retval != aiReturn_SUCCESS)
	{
		LUX_LOG(Utility::logWARNING) << "Failed to retrieve the shininess from file. The field will have its default value.";
	}

	// Shininess strength
	retval = a_Mat.Get(AI_MATKEY_SHININESS_STRENGTH, m_ShininessStrength);
	if (retval != aiReturn_SUCCESS)
	{
		LUX_LOG(Utility::logWARNING) << "Failed to retrieve the shininess strength from file. The field will have its default value.";
	}

	// Refractiveness
	retval = a_Mat.Get(AI_MATKEY_REFRACTI, m_RefractionIndex);
	if (retval != aiReturn_SUCCESS)
	{
		LUX_LOG(Utility::logWARNING) << "Failed to retrieve the refraction index from file. The field will have its default value.";
	}

	// Opacity
	retval = a_Mat.Get(AI_MATKEY_OPACITY, m_Opacity);
	if (retval != aiReturn_SUCCESS)
	{
		LUX_LOG(Utility::logWARNING) << "Failed to retrieve the opacity from file. The field will have its default value.";
	}

	// Reflection
	retval = a_Mat.Get(AI_MATKEY_REFLECTIVITY, m_Reflectivity);
	if (retval != aiReturn_SUCCESS)
	{
		LUX_LOG(Utility::logWARNING) << "Failed to retrieve the reflectivity from file. The field will have its default value.";
	}
}

Lux::Core::MaterialResource::~MaterialResource()
{

}

const Lux::String& Lux::Core::MaterialResource::GetName() const
{
	return m_Name;
}

const Lux::vec4& Lux::Core::MaterialResource::GetDiffuseColor() const
{
	return m_DiffuseColor;
}

const Lux::vec4& Lux::Core::MaterialResource::GetSpecularColor() const
{
	return m_SpecularColor;
}

const Lux::vec4& Lux::Core::MaterialResource::GetEmissiveColor() const
{
	return m_EmissiveColor;
}

const Lux::vec4& Lux::Core::MaterialResource::GetAmbientColor() const
{
	return m_AmbientColor;
}

const Lux::vec4& Lux::Core::MaterialResource::GetReflectiveColor() const
{
	return m_ReflectiveColor;
}

const Lux::vec4& Lux::Core::MaterialResource::GetTransparentColor() const
{
	return m_TransparentColor;
}

const float Lux::Core::MaterialResource::GetShininess() const
{
	return m_Shininess;
}

const float Lux::Core::MaterialResource::GetShininessStrength() const
{
	return m_ShininessStrength;
}

const float Lux::Core::MaterialResource::GetReflectivity() const
{
	return m_Reflectivity;
}

const float Lux::Core::MaterialResource::GetRefraction() const
{
	return m_RefractionIndex;
}

const float Lux::Core::MaterialResource::GetOpacity() const
{
	return m_Opacity;
}

void Lux::Core::MaterialResource::SetName(const String a_Name)
{
	m_Name = a_Name;
}

void Lux::Core::MaterialResource::SetDiffuseColor(const vec4& a_Col)
{
	m_DiffuseColor = a_Col;
}

void Lux::Core::MaterialResource::SetSpecularColor(const vec4& a_Col)
{
	m_SpecularColor = a_Col;
}

void Lux::Core::MaterialResource::SetEmissiveColor(const vec4& a_Col)
{
	m_EmissiveColor = a_Col;
}

void Lux::Core::MaterialResource::SetAmbientColor(const vec4& a_Col)
{
	m_AmbientColor = a_Col;
}

void Lux::Core::MaterialResource::SetReflectiveColor(const vec4& a_Col)
{
	m_ReflectiveColor = a_Col;
}

void Lux::Core::MaterialResource::SetTransparentColor(const vec4& a_Col)
{
	m_TransparentColor = a_Col;
}

void Lux::Core::MaterialResource::SetShininess(float a_Factor)
{
	m_Shininess = a_Factor;
}

void Lux::Core::MaterialResource::SetShininessStrength(float a_Factor)
{
	m_ShininessStrength = a_Factor;
}

void Lux::Core::MaterialResource::SetReflectivity(float a_Factor)
{
	m_Reflectivity = a_Factor;
}

void Lux::Core::MaterialResource::SetRefraction(float a_Factor)
{
	m_RefractionIndex = a_Factor;
}

void Lux::Core::MaterialResource::SetOpacity(float a_Factor)
{
	m_Opacity = a_Factor;
}

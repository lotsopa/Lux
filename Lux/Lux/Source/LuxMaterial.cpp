#include "LuxPCH.h"
#include "LuxMaterial.h"


Lux::Core::Material::Material() :
m_Opacity(1.0f),
m_Reflectivity(0.0f),
m_Shininess(0.0f),
m_ShininessStrength(0.0f),
m_RefractionIndex(0.0f)
{

}

Lux::Core::Material::Material(aiMaterial& a_Mat)
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
		m_DiffuseColor = Utility::ConvertVec4AssimpToGLM(col);
	}

	//Specular
	retval = a_Mat.Get(AI_MATKEY_COLOR_SPECULAR, col);
	if (retval != aiReturn_SUCCESS)
	{
		LUX_LOG(Utility::logWARNING) << "Failed to retrieve the specular color from file. The field will have its default value.";
	}
	else
	{
		m_SpecularColor = Utility::ConvertVec4AssimpToGLM(col);
	}

	// Emissive
	retval = a_Mat.Get(AI_MATKEY_COLOR_EMISSIVE, col);
	if (retval != aiReturn_SUCCESS)
	{
		LUX_LOG(Utility::logWARNING) << "Failed to retrieve the emissive color from file. The field will have its default value.";
	}
	else
	{
		m_EmissiveColor = Utility::ConvertVec4AssimpToGLM(col);
	}

	// Reflective
	retval = a_Mat.Get(AI_MATKEY_COLOR_REFLECTIVE, col);
	if (retval != aiReturn_SUCCESS)
	{
		LUX_LOG(Utility::logWARNING) << "Failed to retrieve the reflective color from file. The field will have its default value.";
	}
	else
	{
		m_ReflectiveColor = Utility::ConvertVec4AssimpToGLM(col);
	}

	// Ambient
	retval = a_Mat.Get(AI_MATKEY_COLOR_AMBIENT, col);
	if (retval != aiReturn_SUCCESS)
	{
		LUX_LOG(Utility::logWARNING) << "Failed to retrieve the ambient color from file. The field will have its default value.";
	}
	else
	{
		m_AmbientColor = Utility::ConvertVec4AssimpToGLM(col);
	}

	// Transparent
	retval = a_Mat.Get(AI_MATKEY_COLOR_TRANSPARENT, col);
	if (retval != aiReturn_SUCCESS)
	{
		LUX_LOG(Utility::logWARNING) << "Failed to retrieve the transparent color from file. The field will have its default value.";
	}
	else
	{
		m_AmbientColor = Utility::ConvertVec4AssimpToGLM(col);
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

Lux::Core::Material::~Material()
{

}

const Lux::String& Lux::Core::Material::GetName() const
{
	return m_Name;
}

const Lux::vec4& Lux::Core::Material::GetDiffuseColor() const
{
	return m_DiffuseColor;
}

const Lux::vec4& Lux::Core::Material::GetSpecularColor() const
{
	return m_SpecularColor;
}

const Lux::vec4& Lux::Core::Material::GetEmissiveColor() const
{
	return m_EmissiveColor;
}

const Lux::vec4& Lux::Core::Material::GetAmbientColor() const
{
	return m_AmbientColor;
}

const Lux::vec4& Lux::Core::Material::GetReflectiveColor() const
{
	return m_ReflectiveColor;
}

const Lux::vec4& Lux::Core::Material::GetTransparentColor() const
{
	return m_TransparentColor;
}

const float Lux::Core::Material::GetShininess() const
{
	return m_Shininess;
}

const float Lux::Core::Material::GetShininessStrength() const
{
	return m_ShininessStrength;
}

const float Lux::Core::Material::GetReflectivity() const
{
	return m_Reflectivity;
}

const float Lux::Core::Material::GetRefraction() const
{
	return m_RefractionIndex;
}

const float Lux::Core::Material::GetOpacity() const
{
	return m_Opacity;
}

void Lux::Core::Material::SetName(const String a_Name)
{
	m_Name = a_Name;
}

void Lux::Core::Material::SetDiffuseColor(const vec4& a_Col)
{
	m_DiffuseColor = a_Col;
}

void Lux::Core::Material::SetSpecularColor(const vec4& a_Col)
{
	m_SpecularColor = a_Col;
}

void Lux::Core::Material::SetEmissiveColor(const vec4& a_Col)
{
	m_EmissiveColor = a_Col;
}

void Lux::Core::Material::SetAmbientColor(const vec4& a_Col)
{
	m_AmbientColor = a_Col;
}

void Lux::Core::Material::SetReflectiveColor(const vec4& a_Col)
{
	m_ReflectiveColor = a_Col;
}

void Lux::Core::Material::SetTransparentColor(const vec4& a_Col)
{
	m_TransparentColor = a_Col;
}

void Lux::Core::Material::SetShininess(float a_Factor)
{
	m_Shininess = a_Factor;
}

void Lux::Core::Material::SetShininessStrength(float a_Factor)
{
	m_ShininessStrength = a_Factor;
}

void Lux::Core::Material::SetReflectivity(float a_Factor)
{
	m_Reflectivity = a_Factor;
}

void Lux::Core::Material::SetRefraction(float a_Factor)
{
	m_RefractionIndex = a_Factor;
}

void Lux::Core::Material::SetOpacity(float a_Factor)
{
	m_Opacity = a_Factor;
}

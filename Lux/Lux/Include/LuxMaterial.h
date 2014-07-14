#ifndef LUX_MATERIAL_H
#define LUX_MATERIAL_H

namespace Lux
{
	class Material
	{
	public:
		Material();
		Material(aiMaterial& a_Mat);
		~Material();

		const String& GetName() const;
		const vec4& GetDiffuseColor() const;
		const vec4& GetSpecularColor() const;
		const vec4& GetEmissiveColor() const;
		const vec4& GetAmbientColor() const;
		const vec4& GetReflectiveColor() const;
		const vec4& GetTransparentColor() const;
		const float GetShininess() const;
		const float GetShininessStrength() const;
		const float GetReflectivity() const;
		const float GetRefraction() const;
		const float GetOpacity() const;

		void SetName(const String a_Name);
		void SetDiffuseColor(const vec4& a_Col);
		void SetSpecularColor(const vec4& a_Col);
		void SetEmissiveColor(const vec4& a_Col);
		void SetAmbientColor(const vec4& a_Col);
		void SetReflectiveColor(const vec4& a_Col);
		void SetTransparentColor(const vec4& a_Col);
		void SetShininess(float a_Factor);
		void SetShininessStrength(float a_Factor);
		void SetReflectivity(float a_Factor);
		void SetRefraction(float a_Factor);
		void SetOpacity(float a_Factor);

	private:
		vec4 m_DiffuseColor;
		vec4 m_SpecularColor;
		vec4 m_EmissiveColor;
		vec4 m_AmbientColor;
		vec4 m_TransparentColor;
		vec4 m_ReflectiveColor;
		float m_Shininess;
		float m_Reflectivity;
		float m_ShininessStrength;
		float m_Opacity;
		float m_RefractionIndex;
		String m_Name;
	};
}

#endif
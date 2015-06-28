#ifndef LUX_MATERIAL_H
#define LUX_MATERIAL_H

namespace Lux
{
	namespace Core
	{
		class MaterialResource;
		class Texture2D;
		class Shader;
	}

	namespace Graphics
	{
		class Material : public Core::Component
		{
		public:
			Material();
			virtual ~Material();

			inline Core::MaterialResource* GetMaterialProperties() { return m_MaterialProperties; }
			inline void SetMaterialProperties(Core::MaterialResource* a_Mat) { m_MaterialProperties = a_Mat; }
			inline void SetDiffuseTexture(Core::Texture2D* a_Tex) { m_DiffuseTexture = a_Tex; }
			inline Core::Texture2D* GetDiffuseTexture() { return m_DiffuseTexture; }
			inline Core::Shader* GetShader() { return m_Shader; }
			inline void SetShader(Core::Shader* a_Shader) { m_Shader = a_Shader; }

		private:
			Core::MaterialResource* m_MaterialProperties;
			Core::Texture2D* m_DiffuseTexture;
			Core::Shader* m_Shader;
		protected:
			void Reset();
			friend class Core::ComponentFactory;
		};
	}
}

#endif
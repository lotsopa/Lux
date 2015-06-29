#ifndef LUX_MATERIAL_H
#define LUX_MATERIAL_H
#include "LuxObserverPtr.h"

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
			Material(const Material& a_Other);
			virtual ~Material();

			inline Core::ObserverPtr<Core::MaterialResource>& GetMaterialProperties() { return m_MaterialProperties; }
			inline void SetMaterialProperties(Core::ObserverPtr<Core::MaterialResource>& a_Mat) { m_MaterialProperties.reset(a_Mat.get()); }
			inline void SetDiffuseTexture(Core::ObserverPtr<Core::Texture2D>& a_Tex) { m_DiffuseTexture.reset(a_Tex.get()); }
			inline Core::ObserverPtr<Core::Texture2D>& GetDiffuseTexture() { return m_DiffuseTexture; }
			inline Core::ObserverPtr<Core::Shader>& GetShader() { return m_Shader; }
			inline void SetShader(Core::ObserverPtr<Core::Shader>& a_Shader) { m_Shader.reset(a_Shader.get()); }

		private:
			Core::ObserverPtr<Core::MaterialResource> m_MaterialProperties;
			Core::ObserverPtr<Core::Texture2D> m_DiffuseTexture;
			Core::ObserverPtr<Core::Shader> m_Shader;
		protected:
			void Reset();
			friend class Core::ComponentFactory;
		};
	}
}

#endif
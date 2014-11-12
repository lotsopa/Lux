#ifndef LUX_MATERIAL_COMPONENT_H
#define LUX_MATERIAL_COMPONENT_H

namespace Lux
{
	namespace Core
	{
		class Material;
	}

	namespace Graphics
	{
		class MaterialComponent : public Core::Component
		{
		public:
			MaterialComponent();
			virtual ~MaterialComponent();

			inline Core::Material* GetMaterial() { return m_Material; }
			inline void SetMaterial(Core::Material* a_Mat) { m_Material = a_Mat; }

		private:
			Core::Material* m_Material;

		protected:
			virtual void Reset();
			friend class ComponentFactory;
		};
	}
}

#endif
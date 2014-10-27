#ifndef LUX_SHADER_COMPONENT_H
#define LUX_SHADER_COMPONENT_H
#include "LuxComponent.h"
namespace Lux
{
	namespace Core
	{
		class Shader;
	}

	namespace Graphics
	{
		class ShaderComponent : public Core::Component
		{
		public:
			ShaderComponent();
			ShaderComponent(Core::Shader* a_Shader);
			virtual ~ShaderComponent();

		protected:
			Core::Shader* m_Shader;
			void Reset();
			friend class Core::ComponentFactory;
		};
	}
}

#endif
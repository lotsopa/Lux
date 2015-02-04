#ifndef LUX_TEXTURE_SAMPLER_DX11_H
#define LUX_TEXTURE_SAMPLER_DX11_H

namespace Lux
{
	namespace Core
	{
		namespace Internal
		{
			class TextureSamplerDX11 : public TextureSampler
			{
			public:
				TextureSamplerDX11(ID3D11DeviceContext* a_Context, TextureSamplerOptions& a_Options);
				~TextureSamplerDX11();

				virtual void Activate(unsigned int a_Unit, ShaderProgram a_Program);
				virtual void Deactivate();

			private:
				Microsoft::WRL::ComPtr<ID3D11SamplerState> m_SamplerState;
				ID3D11DeviceContext* m_DeviceContext;
			};
		}
	}
}

#endif
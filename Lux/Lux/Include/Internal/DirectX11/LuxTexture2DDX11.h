#ifndef LUX_TEXTURE_2D_DX11_H
#define LUX_TEXTURE_2D_DX11_H

namespace Lux
{
	namespace Core
	{
		namespace Internal
		{
			class Texture2DDX11 : public Texture2D
			{
			public:
				Texture2DDX11( ID3D11DeviceContext* a_Context, unsigned int a_ImgWidth, unsigned int a_ImgHeight, unsigned char* a_Bits);
				~Texture2DDX11();

				virtual void Bind(unsigned int a_Slot, const Key& a_Name, Shader* a_Shader, ShaderProgram a_Program);
				virtual void Unbind();

			private:
				Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_TextureResource;
				Microsoft::WRL::ComPtr<ID3D11Texture2D> m_Texture2D;
				ID3D11DeviceContext* m_DeviceContext;
			};
		}
	}
}
#endif
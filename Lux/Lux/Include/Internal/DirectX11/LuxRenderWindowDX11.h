#ifndef LUX_RENDER_WINDOW_DX11_H
#define LUX_RENDER_WINDOW_DX11_H

namespace Lux
{
	namespace Core
	{
		namespace Internal
		{
			class EventListenerDX11;
			class RenderWindowDX11 : public RenderWindow
			{
			public:
				virtual ~RenderWindowDX11();
				virtual bool Initialize(Utility::AppInitOptions& a_AppInitOptions);
				virtual EventListener* GetEventListener();
				virtual void SwapBuffers();
				virtual void Clear();
				virtual void PollEvents();
				virtual void Render(Mesh* a_Mesh);
				virtual const bool IsWindowResized();
				virtual const int GetHeight();
				virtual const int GetWidth();

				inline HWND GetWindowHandle() { return m_WindowHandle; }
				static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
				inline const bool ShouldQuit() { return m_ShouldQuit; }

				inline ID3D11Device* GetDevicePtr() { return m_Device.Get(); }
				inline ID3D11DeviceContext* GetDeviceContextPtr() { return m_DeviceContext.Get(); }
			private:
				RenderWindowDX11();
				HWND m_WindowHandle;
				WNDCLASSEX m_WindowClass;
				HINSTANCE m_AppInstance;
				int m_Width;
				int m_Height;
				int m_SwapInterval;
				bool m_Fullscreen;
				bool m_ShouldQuit;
				bool m_WindowResized;
				String m_Caption;
				unsigned int m_AntiAliasing;
				EventListenerDX11* m_Input;
				Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
				Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext;
				Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;
				Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderBackbuffer;
				Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_DepthStencilState;
				Microsoft::WRL::ComPtr<ID3D11Texture2D> m_DepthStencilBuffer;
				Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DepthStencilView;
				Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_Rasterizer;
				D3D11_VIEWPORT m_ViewPort;
				friend class PlatformDX11;

				bool InitDX11();
				void ResetViewPortTargets();
				void ResizeSwapChain(int width, int height);
			};
		}
	}
}

#endif
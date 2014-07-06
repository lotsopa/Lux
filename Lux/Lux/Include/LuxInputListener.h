#ifndef LUX_INPUT_LISTENER_H
#define LUX_INPUT_LISTENER_H

namespace Lux
{
	class RenderWindow;
	class InputListener
	{
	public:
		InputListener();
		virtual ~InputListener();

		virtual void OnKeyDown(int a_Key, int a_ScanCode);
		virtual void OnKeyUp(int a_Key, int a_ScanCode);
		virtual void OnMouseButtonDown(int a_Btn);
		virtual void OnMouseButtonUp(int a_Btn);
		virtual void OnMouseScroll(double a_OffsetX, double a_OffsetY);
		virtual void OnMouseEnterWindow();
		virtual void OnMouseLeaveWindow();
		virtual void OnMousePositionChanged(double a_X, double a_Y);

	protected:
		RenderWindow* m_WindowOwner;
		void SetWindowOwner(RenderWindow* a_Owner);
		friend class RenderWindow;
	};
}

#endif
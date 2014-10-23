#ifndef LUX_EVENT_LISTENER_H
#define LUX_EVENT_LISTENER_H
#include "LuxComponent.h"

namespace Lux
{
	namespace Core
	{
		class EventListener : public Component
		{
		public:
			EventListener();
			~EventListener();

			virtual void OnKeyDown(int a_Key, int a_ScanCode);
			virtual void OnKeyUp(int a_Key, int a_ScanCode);
			virtual void OnMouseButtonDown(int a_Btn);
			virtual void OnMouseButtonUp(int a_Btn);
			virtual void OnMouseScroll(double a_OffsetX, double a_OffsetY);
			virtual void OnMouseEnterWindow();
			virtual void OnMouseLeaveWindow();
			virtual void OnMousePositionChanged(double a_X, double a_Y);

		protected:
			void Reset();
			friend class ComponentFactory;
		};
	}
}

#endif
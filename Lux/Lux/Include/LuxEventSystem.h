#ifndef LUX_EVENT_SYSTEM_H
#define LUX_EVENT_SYSTEM_H
#include "LuxSystem.h"
namespace Lux
{
	namespace Core
	{
		class EventListener;
		class SceneManager;
		class Component;
		class Entity;
		class RenderWindow;

		class EventSystem : public System
		{
		public:
			EventSystem();
			~EventSystem();

			virtual void ProcessUpdate(const float a_DeltaTime);
			virtual bool Init(SceneManager* a_SceneManager);
			virtual void AddComponent(Component* a_Component, Entity* a_Entity);
			virtual void RemoveComponent(Component* a_Component, Entity* a_Entity);

			void OnKeyDown(int a_Key, int a_ScanCode);
			void OnKeyUp(int a_Key, int a_ScanCode);
			void OnMouseButtonDown(int a_Btn);
			void OnMouseButtonUp(int a_Btn);
			void OnMouseScroll(double a_OffsetX, double a_OffsetY);
			void OnMouseEnterWindow();
			void OnMouseLeaveWindow();
			void OnMousePositionChanged(double a_X, double a_Y);

			inline RenderWindow* GetRenderWindow() { return m_RenderWindow; }

		private:
			RenderWindow* m_RenderWindow;
			typedef std::map<Entity*, EventListener*> EntityMap;
			EntityMap m_EntityMap;
		};
	}
}

#endif
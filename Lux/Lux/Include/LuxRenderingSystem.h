#ifndef LUX_RENDERING_SYSTEM_H
#define LUX_RENDERING_SYSTEM_H

namespace Lux
{
	class SceneManager;
	class RenderWindow;

	class RenderingSystem : public System
	{
		// TODO
	public:
		RenderingSystem();
		~RenderingSystem();

		virtual void ProcessUpdate(const float a_DeltaTime);
		virtual bool Init(SceneManager* a_SceneManager);
	private:
		RenderWindow* m_RenderWindow;
	};
}

#endif
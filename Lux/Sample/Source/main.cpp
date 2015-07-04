#include "TestApp.h"
//#include <vld.h>

void CheckResult(bool res)
{
	if (!res)
	{
		exit(-1);
	}
}

int main(int argc, char* argv[])
{
	TestApp app;
	Lux::Utility::AppInitOptions initOptions;
	initOptions.m_WindowCaption = "Sample";
	initOptions.m_AntiAliasing = 4;
	initOptions.m_ScreenSwapInterval = 1;
	initOptions.m_PlatformType = Lux::Utility::DIRECTX_11;
	bool res = app.Initialize(initOptions);
	CheckResult(res);
	res = app.LoadContent();
	CheckResult(res);
	app.Run();
	res = app.UnloadContent();
	CheckResult(res);

	return 0;
}
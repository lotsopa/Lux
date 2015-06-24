#ifndef TEST_APP_H
#define TEST_APP_H
#include "Lux.h"

class TestApp : public Lux::Application
{
public:
	TestApp();
	~TestApp();

	virtual bool LoadContent();
	virtual bool UnloadContent();

	virtual bool Update(const float a_DeltaTime);

	virtual bool LoadComponentTypes();
	virtual bool LoadSystemTypes();

private:

protected:

};

#endif
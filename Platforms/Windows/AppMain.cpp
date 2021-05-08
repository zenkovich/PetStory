#include "o2/stdafx.h"
#include "TestApplication.h"
#include "o2/O2.h"

int main()
{
	INITIALIZE_O2;

	GameApplication* app = mnew GameApplication();
	app->Initialize();
	app->Launch();

	return 0;
}

#include "GameApplication.h"
#include "o2/O2.h"

int main()
{
	INITIALIZE_O2;

	GameApplication* app = mnew GameApplication();
	app->Initialize();
	app->Launch();
	delete app;

	return 0;
}

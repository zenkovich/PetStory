#include "GameApplication.h"
#include "o2/O2.h"

extern void InitializeTypesGameLib();

int main()
{
	InitializeTypesGameLib();
	INITIALIZE_O2;

	GameApplication app;
	app.Initialize();
	app.Launch();

	return 0;
}

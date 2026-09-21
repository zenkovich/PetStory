#include "App/GameApplication.h"
#include "o2/O2.h"

#if __has_include("o2libs/o2libs.h")
#include "o2libs/o2libs.h"
#else
#define O2LIBS_INITIALIZE_TYPES
#define O2LIBS_START
#endif

extern void InitializeTypesGameLib();
extern void InitializeTypesPetStoryLib();

using namespace o2;

int main(int argc, char * argv[]) {
	INITIALIZE_O2;
	InitializeTypesGameLib();
	InitializeTypesPetStoryLib();
	O2LIBS_INITIALIZE_TYPES;

	auto app = mmake<GameApplication>();
	app->Run(argc, argv);

	return 0;
}

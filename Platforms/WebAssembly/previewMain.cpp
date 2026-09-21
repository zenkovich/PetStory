#include "o2/stdafx.h"
#include "o2/O2.h"
#include "o2/Utils/Memory/MemoryAnalyzer.h"

#include "App/GameApplication.h"

#include <unistd.h>

#if __has_include("o2libs/o2libs.h")
#include "o2libs/o2libs.h"
#else
#define O2LIBS_INITIALIZE_TYPES
#define O2LIBS_START
#endif

using namespace o2;

extern void InitializeTypesGameLib();
extern void InitializeTypesPetStoryLib();
extern void InitializeTypesAssetsBuildTool();

// The game client of the web editor: the same GameApplication the players get,
// but reading the project from the session working copy the page streams into
// MEMFS instead of a packed .data. That is what lets it rebuild the assets and
// restart with the changes the agent (or the editor next door) has just made.
int main()
{
    // Built from the editor-enabled tree, so the engine's asset paths are the
    // project-relative ones: running from Bin/WebAssembly inside the mounted
    // project makes ../../BuiltAssets/WebAssembly resolve as on desktop.
    chdir("/project/Bin/WebAssembly");

    o2::MemoryAnalyzer::enabledObjectsTracking = false;
    INITIALIZE_O2;
    InitializeTypesGameLib();
    InitializeTypesPetStoryLib();
    O2LIBS_INITIALIZE_TYPES;
    InitializeTypesAssetsBuildTool();

    auto app = mmake<GameApplication>();
    app->Initialize();
    O2LIBS_START;
    app->Launch();

    return 0;
}

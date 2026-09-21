#include "o2/stdafx.h"
#include "o2/O2.h"
#include "o2/Utils/Memory/MemoryAnalyzer.h"
#include "o2Editor/EditorApplication.h"
#include "o2Editor/EditorConfig.h"
#include "o2Editor/ToolsPanel.h"
#include "o2Editor/Windows/WindowsManager.h"

#if __has_include("o2libs/o2libs.h")
#include "o2libs/o2libs.h"
#else
#define O2LIBS_INITIALIZE_TYPES
#define O2LIBS_START
#endif

using namespace o2;

DECLARE_SINGLETON(Editor::WindowsManager);
DECLARE_SINGLETON(Editor::EditorConfig);
DECLARE_SINGLETON(Editor::ToolsPanel);

extern void InitializeTypeso2Editor();
extern void InitializeTypesGameLib();
extern void InitializeTypesPetStoryLib();
extern void InitializeTypesEditorLib();

int main()
{
    o2::MemoryAnalyzer::enabledObjectsTracking = false;
	INITIALIZE_O2;
    InitializeTypesGameLib();
    InitializeTypesPetStoryLib();
    O2LIBS_INITIALIZE_TYPES;
    InitializeTypeso2Editor();
    InitializeTypesEditorLib();
    o2::MemoryAnalyzer::enabledObjectsTracking = true;

	auto app = mmake<Editor::EditorApplication>();
    app->Initialize();
    O2LIBS_START;
    app->Launch();

	return 0;
}

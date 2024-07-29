#include "o2/stdafx.h"
#include "o2/O2.h"
#include "o2/Utils/Memory/MemoryAnalyzer.h"
#include "o2/Utils/System/Time/Timer.h"
#include "o2Editor/Core/EditorApplication.h"
#include "o2Editor/Core/EditorConfig.h"
#include "o2Editor/Core/ToolsPanel.h"
#include "o2Editor/Core/WindowsSystem/WindowsManager.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"

using namespace o2;

DECLARE_SINGLETON(Editor::WindowsManager);
DECLARE_SINGLETON(Editor::EditorConfig);
DECLARE_SINGLETON(Editor::ToolsPanel);

extern void InitializeTypeso2Editor();
extern void InitializeTypesGameLib();

int main()
{
    o2::MemoryAnalyzer::enabledObjectsTracking = false;
    InitializeTypeso2Editor();
    InitializeTypesGameLib();
	INITIALIZE_O2;
    o2::MemoryAnalyzer::enabledObjectsTracking = true;

	auto app = mmake<Editor::EditorApplication>();
    app->Initialize();
    app->Launch();

	return 0;
}

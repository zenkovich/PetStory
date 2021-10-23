#include "o2Editor/Core/EditorApplication.h"
#include "o2/O2.h"

using namespace o2;

DECLARE_SINGLETON(Editor::WindowsManager);
DECLARE_SINGLETON(Editor::EditorConfig);
DECLARE_SINGLETON(Editor::ToolsPanel);

int main(int argc, const char * argv[]) {
	INITIALIZE_O2;
	
	Editor::EditorApplication* app = mnew Editor::EditorApplication();
	app->Initialize();
	app->Launch();
	delete app;

	return 0;
}

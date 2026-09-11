#include "o2Editor/stdafx.h"
#include "webSceneBridge.h"

#include <emscripten.h>

#include "o2/Assets/Types/SceneAsset.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2Editor/EditorApplication.h"
#include "o2Editor/Windows/GameWindow/GameWindow.h"
#include "o2Editor/Windows/WindowsManager.h"

using namespace o2;

static Editor::EditorApplication* EditorApp()
{
    return (Editor::EditorApplication*)(o2::Application::InstancePtr());
}

// Play mode control for the page (the AI agent drives the game this way, since
// clicking the editor chrome from script is unreliable)
extern "C" EMSCRIPTEN_KEEPALIVE void o2_web_set_play(int playing)
{
    EditorApp()->SetPlaying(playing != 0);
}

extern "C" EMSCRIPTEN_KEEPALIVE int o2_web_is_playing()
{
    return EditorApp()->IsPlaying() ? 1 : 0;
}

// Scene control for the page: the agent cannot click the editor chrome, so it
// opens and saves scenes through here
extern "C" EMSCRIPTEN_KEEPALIVE void o2_web_open_scene(const char* path)
{
    EditorApp()->LoadScene(o2::AssetRef<o2::SceneAsset>(o2::String(path)));
}

extern "C" EMSCRIPTEN_KEEPALIVE void o2_web_save_scene()
{
    EditorApp()->SaveScene();
}

// ---------------------------------------------------------------- bridge hooks

String WebBridge::OpenSceneName()
{
    return EditorApp()->GetLoadedSceneName();
}

bool WebBridge::IsPlaying()
{
    return EditorApp()->IsPlaying();
}

String WebBridge::ViewInfoExtra()
{
    if (auto gameWindow = Editor::WindowsManager::Instance().GetWindow<Editor::GameWindow>())
    {
        if (auto view = gameWindow->GetGameViewWidget())
        {
            RectF r = view->layout->GetWorldRect();
            return ",\"gameView\":{\"left\":" + (String)r.left + ",\"top\":" + (String)r.top +
                   ",\"right\":" + (String)r.right + ",\"bottom\":" + (String)r.bottom + "}";
        }
    }

    return String();
}

#include "o2/stdafx.h"
#include "webSceneBridge.h"

#include <emscripten.h>

#include "App/GameApplication.h"
#include "o2/Application/Application.h"
#include "o2/Render/Render.h"
#include "o2/Scene/Scene.h"

using namespace o2;

static GameApplication* GameApp()
{
    return (GameApplication*)(o2::Application::InstancePtr());
}

// The preview's answer to the editor's play mode: there is nothing to start or
// stop, so the page restarts the client instead — the game runs from the top
// with whatever the assets look like now (rebuild first to pick up edits).
extern "C" EMSCRIPTEN_KEEPALIVE void o2_web_restart()
{
    GameApp()->Restart();
}

// ---------------------------------------------------------------- bridge hooks

String WebBridge::OpenSceneName()
{
    return GameApp()->GetScenePath();
}

bool WebBridge::IsPlaying()
{
    return true;
}

String WebBridge::ViewInfoExtra()
{
    // The game fills the canvas, so the view rect is the canvas — spelled out
    // anyway, so the agent reads the same shape in both modes
    Vec2F resolution = o2Render.GetResolution();
    return ",\"gameView\":{\"left\":0,\"top\":0,\"right\":" + (String)resolution.x +
           ",\"bottom\":" + (String)resolution.y + "}";
}

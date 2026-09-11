#pragma once

#include "o2/Utils/Types/String.h"

// The part of the page-callable bridge that is the same in the editor and in the
// game preview: reading the scene, running scripts, rebuilding assets. Whatever
// only one of them can answer comes from the hooks below, implemented next to
// each entry point (editorAssetsBuild.cpp / previewBridge.cpp).
namespace WebBridge
{
    // Scene the client currently has open, as the agent should see it
    o2::String OpenSceneName();

    // Whether the game is running right now (the editor's play mode; always true
    // in the preview, where the game is all there is)
    bool IsPlaying();

    // Extra members for o2_web_view_info, e.g. the editor's Game window rect.
    // Either empty or starting with a comma, so it can be pasted into the object
    o2::String ViewInfoExtra();

    // Copy of a string into a buffer the page frees after reading it
    char* Dup(const o2::String& str);

    // JSON-escaped copy of a string
    o2::String Escape(const o2::String& str);
}

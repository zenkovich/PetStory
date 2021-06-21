#include "GameApplication.h"

#include "o2/Render/Render.h"
#include "o2/Scene/Scene.h"

void GameApplication::OnStarted()
{
	o2Scene.Load("Assets/test.scn");
	o2Application.SetWindowSize(Vec2I(750, 1334));
}

void GameApplication::OnUpdate(float dt)
{
}

void GameApplication::OnDraw()
{
	o2Render.Clear();
	o2Render.SetCamera(Camera::Default());
}


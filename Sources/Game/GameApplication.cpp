#include "GameApplication.h"

#include "o2/Render/Render.h"


void GameApplication::OnStarted()
{
}

void GameApplication::OnUpdate(float dt)
{
}

void GameApplication::OnDraw()
{
	o2Render.Clear();
	o2Render.SetCamera(Camera::Default());
}


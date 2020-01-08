#include "TestApplication.h"

#include "o2/Render/Render.h"


void TestApplication::OnStarted()
{
}

void TestApplication::OnUpdate(float dt)
{
}

void TestApplication::OnDraw()
{
	o2Render.Clear();
	o2Render.SetCamera(Camera::Default());
}


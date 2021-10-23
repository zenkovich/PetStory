#include "GameApplication.h"

#include "o2/Render/Render.h"
#include "o2/Scene/Scene.h"
#include "o2/Application/Input.h"

void GameApplication::OnStarted()
{
	o2Scene.Load("Assets/test.scn");
	o2Application.SetWindowSize(Vec2I(750, 1334));
}

void GameApplication::OnUpdate(float dt)
{
	o2Application.windowCaption = String("Pet story") +
	"; FPS: " + (String)((int)o2Time.GetFPS()) +
	" Cursor: " + (String)o2Input.GetCursorPos();
}

void GameApplication::OnDraw()
{
	float scale = o2Application.GetGraphicsScale();
	Camera camera = Camera::Default();
	camera.scale = Vec2F(1.0f/scale, 1.0f/scale);
	o2Render.camera = camera;
}


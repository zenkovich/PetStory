#pragma once

#include "o2/Application/Application.h"

using namespace o2;

class TestApplication: public Application
{
protected:
	// Calls when application is starting
	void OnStarted();

	// Called on updating
	void OnUpdate(float dt);

	// Called on drawing
	void OnDraw();
};

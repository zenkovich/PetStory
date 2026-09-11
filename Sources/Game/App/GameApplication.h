#pragma once

#include "o2/Application/Application.h"

using namespace o2;

class GameApplication: public Application
{
public:
	// Default constructor
	GameApplication(RefCounter* refCounter);

	// Reloads the scene from scratch, as if the game had just started. The web
	// preview restarts the client this way after the assets were rebuilt
	void Restart();

	// Scene the game starts with, for the tools that report what is open
	const String& GetScenePath() const;

protected:
	// Calls when application is starting
	void OnStarted() override;

	// Called on updating
	void OnUpdate(float dt) override;

	// Called on drawing
	void OnDraw() override;

protected:
	String mScenePath = "Boot.scn"; // What the game starts with, and restarts into

	// Draws scene
	void DrawScene() override;
};

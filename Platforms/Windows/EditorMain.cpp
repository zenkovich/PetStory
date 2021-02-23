#include "o2/stdafx.h"
#include "o2/O2.h"
#include "o2Editor/Core/EditorApplication.h"
#include "o2Editor/Core/EditorConfig.h"
#include "o2Editor/Core/ToolsPanel.h"
#include "o2Editor/Core/WindowsSystem/WindowsManager.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "o2/Utils/System/Time/Timer.h"

using namespace o2;

DECLARE_SINGLETON(Editor::WindowsManager);
DECLARE_SINGLETON(Editor::EditorConfig);
DECLARE_SINGLETON(Editor::ToolsPanel);


template<typename base>
struct A: public base
{
	A& DoSmth() { base::DoSmth(); printf("A"); return *this; }

	template<template<typename> class mixin>
	mixin<A<base>> Mix()
	{
		return MixImpl<mixin, A<base>>();
	}
};

template<typename base>
struct B: public base
{
	B& DoSmth() { base::DoSmth(); printf("B"); return *this; }

	template<template<typename> class mixin>
	mixin<B<base>> Mix()
	{
		return MixImpl<mixin, B<base>>();
	}
};

struct Basic
{
	Basic& DoSmth() { printf("Basic"); return *this; }

	template<template<typename> class mixin>
	mixin<Basic> Mix()
	{
		return MixImpl<mixin, Basic>();
	}

	template<template<typename> class mixin, class basemix>
	mixin<basemix> MixImpl()
	{
		return mixin<basemix>();
	}
};

int main()
{
	INITIALIZE_O2;

	Basic x;
	x.Mix<A>().Mix<B>().DoSmth(); 
	printf("\n");
	x.Mix<B>().Mix<A>().DoSmth();
	printf("\n");

	Editor::EditorApplication* app = mnew Editor::EditorApplication();
	app->Initialize();
	app->Launch();

	return 0;
}

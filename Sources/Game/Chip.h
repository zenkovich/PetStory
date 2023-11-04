#pragma once
#include "o2/Events/CursorAreaEventsListener.h"
#include "o2/Scene/Component.h"
#include "o2/Scene/ComponentRef.h"
#include "o2/Scene/Components/ImageComponent.h"

using namespace o2;

class Chip: public Component, public CursorAreaEventsListener
{
public:
	bool IsUnderPoint(const Vec2F& point) override;

	void OnStart() override;

	SERIALIZABLE(Chip);

private:
	String mType;                    // @SERIALIZABLE @EDITOR_PROPERTY
	float  mGroupingRadius = 230.0f; // @SERIALIZABLE @EDITOR_PROPERTY

	Ref<ImageComponent> mImage;

private:
	 void OnCursorReleased(const Input::Cursor& cursor) override;
};

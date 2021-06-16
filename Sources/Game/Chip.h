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

CLASS_BASES_META(Chip)
{
	BASE_CLASS(Component);
	BASE_CLASS(CursorAreaEventsListener);
}
END_META;
CLASS_FIELDS_META(Chip)
{
	FIELD().EDITOR_PROPERTY_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE().NAME(mType).PRIVATE();
	FIELD().EDITOR_PROPERTY_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(230.0f).NAME(mGroupingRadius).PRIVATE();
	FIELD().NAME(mImage).PRIVATE();
}
END_META;
CLASS_METHODS_META(Chip)
{

	PUBLIC_FUNCTION(bool, IsUnderPoint, const Vec2F&);
	PUBLIC_FUNCTION(void, OnStart);
	PRIVATE_FUNCTION(void, OnCursorReleased, const Input::Cursor&);
}
END_META;

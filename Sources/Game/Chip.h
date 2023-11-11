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
// --- META ---

CLASS_BASES_META(Chip)
{
    BASE_CLASS(Component);
    BASE_CLASS(CursorAreaEventsListener);
}
END_META;
CLASS_FIELDS_META(Chip)
{
    FIELD().PRIVATE().EDITOR_PROPERTY_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE().NAME(mType);
    FIELD().PRIVATE().EDITOR_PROPERTY_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(230.0f).NAME(mGroupingRadius);
    FIELD().PRIVATE().NAME(mImage);
}
END_META;
CLASS_METHODS_META(Chip)
{

    FUNCTION().PUBLIC().SIGNATURE(bool, IsUnderPoint, const Vec2F&);
    FUNCTION().PUBLIC().SIGNATURE(void, OnStart);
    FUNCTION().PRIVATE().SIGNATURE(void, OnCursorReleased, const Input::Cursor&);
}
END_META;
// --- END META ---

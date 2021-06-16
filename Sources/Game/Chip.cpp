#include "Chip.h"
#include "o2/Scene/Actor.h"

bool Chip::IsUnderPoint(const Vec2F& point)
{
	if (mImage)
		return mImage->IsUnderPoint(point);

	return false;
}

void Chip::OnStart()
{
	mImage = mOwner->GetComponent<ImageComponent>();
	if (mImage)
		mImage->Sprite::onDraw = [&] { OnDrawn(); };
}

void Chip::OnCursorReleased(const Input::Cursor& cursor)
{
	auto parent = mOwner->GetParent();
	if (!parent)
		return;

	Vector<Chip*> group;
	group.Add(this);

	for (auto child : parent->GetChildren())
	{
		if (child == GetOwnerActor())
			continue;

		if (auto chip = child->GetComponent<Chip>())
		{
			if (chip->mType == mType &&
				(child->transform->worldPosition.Get() - mOwner->transform->worldPosition).Length() < chip->mGroupingRadius + mGroupingRadius)
			{
				group.Add(chip);
			}
		}
	}

	if (group.Count() < 3)
		return;

	for (auto chip : group)
		o2Scene.DestroyActor(chip->GetOwnerActor());
}

DECLARE_CLASS(Chip);

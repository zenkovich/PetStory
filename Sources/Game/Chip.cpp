#include "Chip.h"

#include "o2/Scene/Scene.h"
#include "o2/Render/Render.h"

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
		mImage->DrawableComponent::onDraw = [&] { OnDrawn(); };
}

void Chip::OnCursorReleased(const Input::Cursor& cursor)
{
	auto parent = mOwner->GetParent();
	if (!parent)
		return;

	Vector<Chip*> group;
	Vector<Chip*> iterationChipGroup;
	iterationChipGroup.Add(this);

	while (!iterationChipGroup.IsEmpty())
	{
		group.Add(iterationChipGroup);

		Vector<Chip*> prevIterationChipGroup = iterationChipGroup;
		iterationChipGroup.Clear();

		for (auto groupChip : prevIterationChipGroup)
		{
			for (auto child : parent->GetChildren())
			{
				if (child == GetOwnerActor())
					continue;

				if (auto chip = child->GetComponent<Chip>())
				{
					if (chip->mType != mType)
						continue;

					if (group.Contains(chip))
						continue;

					float distance = (child->transform->worldPosition.Get() - mOwner->transform->worldPosition).Length();
					if (distance > chip->mGroupingRadius + mGroupingRadius)
						continue;
						
					iterationChipGroup.Add(chip);
				}
			}
		}
	}

	if (group.Count() < 2)
 		return;

	for (auto chip : group)
		o2Scene.DestroyActor(chip->GetOwnerActor());
}

DECLARE_CLASS(Chip);

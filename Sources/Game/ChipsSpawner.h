#pragma once
#include "o2/Assets/Types/ActorAsset.h"
#include "o2/Scene/ActorRef.h"
#include "o2/Scene/Component.h"
#include "o2/Utils/Editor/Attributes/EditorPropertyAttribute.h"
#include "o2/Utils/Math/Math.h"

using namespace o2;

class ChipsSpawnerComponent: public Component
{
public:
	// Updates component, checks count of chips
	void Update(float dt) override;

	SERIALIZABLE(ChipsSpawnerComponent);

private:
	float         mSpawnDelay = 0.2f;  // @SERIALIZABLE @EDITOR_PROPERTY
	int           mMaxChipsCount = 20; // @SERIALIZABLE @EDITOR_PROPERTY
	ActorRef      mSpawnContainer;     // @SERIALIZABLE @EDITOR_PROPERTY
	ActorRef      mSpawnZone;          // @SERIALIZABLE @EDITOR_PROPERTY
	ActorAssetRef mChipProto;          // @SERIALIZABLE @EDITOR_PROPERTY

	float mAccumulatedTimer = 0.0f;

private:
	void CheckChipsCount();
};
// --- META ---

CLASS_BASES_META(ChipsSpawnerComponent)
{
	BASE_CLASS(Component);
}
END_META;
CLASS_FIELDS_META(ChipsSpawnerComponent)
{
	FIELD().PRIVATE().EDITOR_PROPERTY_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(0.2f).NAME(mSpawnDelay);
	FIELD().PRIVATE().EDITOR_PROPERTY_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(20).NAME(mMaxChipsCount);
	FIELD().PRIVATE().EDITOR_PROPERTY_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE().NAME(mSpawnContainer);
	FIELD().PRIVATE().EDITOR_PROPERTY_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE().NAME(mSpawnZone);
	FIELD().PRIVATE().EDITOR_PROPERTY_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE().NAME(mChipProto);
	FIELD().PRIVATE().DEFAULT_VALUE(0.0f).NAME(mAccumulatedTimer);
}
END_META;
CLASS_METHODS_META(ChipsSpawnerComponent)
{

	FUNCTION().PUBLIC().SIGNATURE(void, Update, float);
	FUNCTION().PRIVATE().SIGNATURE(void, CheckChipsCount);
}
END_META;
// --- END META ---

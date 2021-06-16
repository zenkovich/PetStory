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

CLASS_BASES_META(ChipsSpawnerComponent)
{
	BASE_CLASS(Component);
}
END_META;
CLASS_FIELDS_META(ChipsSpawnerComponent)
{
	FIELD().EDITOR_PROPERTY_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(0.2f).NAME(mSpawnDelay).PRIVATE();
	FIELD().EDITOR_PROPERTY_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(20).NAME(mMaxChipsCount).PRIVATE();
	FIELD().EDITOR_PROPERTY_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE().NAME(mSpawnContainer).PRIVATE();
	FIELD().EDITOR_PROPERTY_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE().NAME(mSpawnZone).PRIVATE();
	FIELD().EDITOR_PROPERTY_ATTRIBUTE().SERIALIZABLE_ATTRIBUTE().NAME(mChipProto).PRIVATE();
	FIELD().DEFAULT_VALUE(0.0f).NAME(mAccumulatedTimer).PRIVATE();
}
END_META;
CLASS_METHODS_META(ChipsSpawnerComponent)
{

	PUBLIC_FUNCTION(void, Update, float);
	PRIVATE_FUNCTION(void, CheckChipsCount);
}
END_META;

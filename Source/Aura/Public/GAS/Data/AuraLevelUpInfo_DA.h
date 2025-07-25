// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AuraLevelUpInfo_DA.generated.h"

USTRUCT( BlueprintType )
struct FAuraLevelUpInfo
{
	GENERATED_BODY()
public:
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	int LevelUpXpRequirement = 0;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	int AttributePointsReward = 1;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	int SpellPointsReward = 1;
};

/**
 *
 */
UCLASS()
class AURA_API UAuraLevelUpInfo_DA : public UDataAsset
{
	GENERATED_BODY()

public:
	FAuraLevelUpInfo FindLevelUpInfoForXp( int32 XpAmount ) const;
	int32 FindLevelForXp( int32 XpAmount ) const;
	FAuraLevelUpInfo FindLevelUpInfoForLevel( int32 Level ) const;

protected:
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "LevelUpInfo" )
	TArray<FAuraLevelUpInfo> LevelUpInfos;
};

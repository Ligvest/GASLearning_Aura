// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AuraCharacterClassInfoDA.generated.h"

class UGameplayEffect;

UENUM( BlueprintType )
enum class ECharacterClass : uint8
{
	Default,
	Elementalist,
	Warrior,
	Ranger
};

USTRUCT( BlueprintType )
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY( EditDefaultsOnly, Category = "Class Defaults" )
	TSubclassOf<UGameplayEffect> InitPrimaryAttributesEffectClass;
};

/**
 *
 */
UCLASS()
class AURA_API UAuraCharacterClassInfoDA : public UDataAsset
{
	GENERATED_BODY()

protected:
	UPROPERTY( EditDefaultsOnly, Category = "Common Class Defaults" )
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassToDefaultInfo;

public:
	FCharacterClassDefaultInfo GetClassDefaultInfo( ECharacterClass CharacterClass ) { return CharacterClassToDefaultInfo.FindChecked( CharacterClass ); };

	UPROPERTY( EditDefaultsOnly, Category = "Common Class Defaults" )
	TSubclassOf<UGameplayEffect> InitSecondaryAttributesEffectClass;

	UPROPERTY( EditDefaultsOnly, Category = "Common Class Defaults" )
	TSubclassOf<UGameplayEffect> InitVitalAttributesEffectClass;
};

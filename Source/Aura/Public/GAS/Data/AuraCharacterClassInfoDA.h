// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "ScalableFloat.h"
#include "Engine/DataAsset.h"
#include "AuraCharacterClassInfoDA.generated.h"

class UGameplayAbility;
class UGameplayEffect;

UENUM( BlueprintType )
enum class ECharacterClass : uint8
{
	Empty,
	Player,
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

	UPROPERTY( EditDefaultsOnly, Category = "Common Class Defaults" )
	TSubclassOf<UGameplayEffect> InitPrimaryAttributes_SetByCallerClass;

	UPROPERTY( EditDefaultsOnly, Category = "Class Defaults" )
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilityClasses;

	UPROPERTY( EditDefaultsOnly, Category = "Class Defaults" )
	FScalableFloat XpReward;
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
	const FCharacterClassDefaultInfo& GetClassDefaultInfo( ECharacterClass CharacterClass ) { return CharacterClassToDefaultInfo.FindChecked( CharacterClass ); };

	UPROPERTY( EditDefaultsOnly, Category = "Common Class Defaults" )
	TSubclassOf<UGameplayEffect> InitSecondaryAttributesEffectClass;

	UPROPERTY( EditDefaultsOnly, Category = "Common Class Defaults" )
	TSubclassOf<UGameplayEffect> InitVitalAttributesEffectClass;

	UPROPERTY( EditDefaultsOnly, Category = "Common Class Defaults" )
	TObjectPtr<UCurveTable> CalculationCoefficientsCT;
};

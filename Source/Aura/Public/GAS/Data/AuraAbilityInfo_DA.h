// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AuraAbilityInfo_DA.generated.h"

class UGameplayAbility;
USTRUCT( BlueprintType )
struct FAuraAbilityInfo
{
	GENERATED_BODY()

	// Not BP Exposed
	UPROPERTY( BlueprintReadOnly )
	FGameplayTag AbilityStatusTag = FGameplayTag();

	UPROPERTY( BlueprintReadOnly )
	FGameplayTag InputTag = FGameplayTag();

	// BP Exposed
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	FGameplayTag AbilityTag = FGameplayTag();

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	FGameplayTag AbilityTypeTag = FGameplayTag();

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	FGameplayTag CooldownTag = FGameplayTag();

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	TObjectPtr<const UTexture2D> Icon = nullptr;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	TObjectPtr<const UMaterialInterface> BackgroundMaterial = nullptr;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	int32 LevelRequirement = 1;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	TSubclassOf<UGameplayAbility> AbilityClass = nullptr;
};

/**
 *
 */
UCLASS()
class AURA_API UAuraAbilityInfo_DA : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityInformation" )
	TArray<FAuraAbilityInfo> AbilityInformation;

	FAuraAbilityInfo FindAbilityInfoForTag( const FGameplayTag AbilityTag, bool bLogNotFound = false ) const;
};

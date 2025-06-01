// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AuraAttributeInfoDataAsset.generated.h"

/**
 *
 */
USTRUCT( BlueprintType )
struct FAuraAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	FGameplayTag Tag;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	FText Name;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	FText Description;

	UPROPERTY( BlueprintReadOnly )
	float Value;
};
UCLASS()
class AURA_API UAuraAttributeInfoDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	// Get attribute info from the AttributeInfos initialized in BP
	FAuraAttributeInfo GetAttributeInfoByTag( const FGameplayTag& Tag );

protected:
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	TArray<FAuraAttributeInfo> AttributeInfos;
};

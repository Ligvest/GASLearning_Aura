// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputAction.h"
#include "Engine/DataAsset.h"
#include "AuraInputConfig.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UAuraInputConfig : public UDataAsset
{
	GENERATED_BODY()

	UInputAction* GetInputActionByGameplayTag( const FGameplayTag GameplayTag );

public:
	UPROPERTY( EditDefaultsOnly )
	TMap<FGameplayTag, UInputAction*> TagsToInputActions;
};

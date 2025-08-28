// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/AuraDamageGameplayAbility.h"
#include "ArcaneShards.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UArcaneShards : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
public:
	virtual FString GetDescription( int32 Level ) const override;
	virtual FString GetNextLevelDescription( int32 Level ) const override;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	int32 MaxNumShards = 11;
};

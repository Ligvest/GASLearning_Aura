// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/AuraDamageGameplayAbility.h"
#include "AuraFireBlast.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UAuraFireBlast : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
public:
	virtual FString GetDescription( int32 Level ) const override;
	virtual FString GetNextLevelDescription( int32 Level ) const override;

protected:
	UPROPERTY( EditDefaultsOnly, Category = "FireBlast" )
	int32 NumFireBalls = 12;
};

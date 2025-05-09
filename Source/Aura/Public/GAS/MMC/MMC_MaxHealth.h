// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "GAS/MMC/MMC_LevelBonus.h"
#include "MMC_MaxHealth.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UMMC_MaxHealth : public UMMC_LevelBonus
{
	GENERATED_BODY()
public:
	UMMC_MaxHealth();

protected:
	virtual FGameplayAttribute GetBaseAttribute() const;
	virtual float CalculateBaseMagnitude_Implementation( const FGameplayEffectSpec& Spec ) const override;
};

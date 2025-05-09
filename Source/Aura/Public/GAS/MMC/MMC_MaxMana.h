// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "GAS/MMC/MMC_LevelBonus.h"
#include "MMC_MaxMana.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UMMC_MaxMana : public UMMC_LevelBonus
{
	GENERATED_BODY()
public:
	UMMC_MaxMana();

protected:
	virtual FGameplayAttribute GetBaseAttribute() const;
	virtual float CalculateBaseMagnitude_Implementation( const FGameplayEffectSpec& Spec ) const override;
};

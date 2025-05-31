// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_LevelBonus.generated.h"

struct FCalcParams
{
	float Coefficient = 0.f;
	float PostMultiplyAdditiveValue = 0.f;
	float PreMultiplyAdditiveValue = 0.f;
	float ActorLevelCoefficient = 0.f;
};
/**
 *
 */
UCLASS()
class AURA_API UMMC_LevelBonus : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:
	virtual float CalculateBaseMagnitude_Implementation( const FGameplayEffectSpec& Spec ) const override;

	UMMC_LevelBonus();

protected:
	virtual FGameplayAttribute GetBaseAttribute() const;
	// A function to call from ctor
	void InitBaseAttributeDef();

protected:
	FGameplayEffectAttributeCaptureDefinition BaseAttributeDef;
	FCalcParams CalcParams;
};

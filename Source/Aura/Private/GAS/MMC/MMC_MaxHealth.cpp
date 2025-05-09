// Dovzhik Tolya

#include "GAS/MMC/MMC_MaxHealth.h"
#include "GAS/AuraAttributeSet.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	InitBaseAttributeDef();
	CalcParams.Coefficient = 2.5f;
	CalcParams.PostMultiplyAdditiveValue = 80.f;
	CalcParams.PreMultiplyAdditiveValue = 0.f;
	CalcParams.ActorLevelCoefficient = 10.f;
}

FGameplayAttribute UMMC_MaxHealth::GetBaseAttribute() const
{
	return UAuraAttributeSet::GetVigorAttribute();
}
float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation( const FGameplayEffectSpec& Spec ) const
{
	return Super::CalculateBaseMagnitude_Implementation( Spec );
}
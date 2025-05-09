// Dovzhik Tolya

#include "GAS/MMC/MMC_MaxMana.h"

#include "GAS/AuraAttributeSet.h"
UMMC_MaxMana::UMMC_MaxMana()
{
	InitBaseAttributeDef();
	CalcParams.Coefficient = 2.5f;
	CalcParams.PostMultiplyAdditiveValue = 50.f;
	CalcParams.PreMultiplyAdditiveValue = 0.f;
	CalcParams.ActorLevelCoefficient = 15.f;
}
FGameplayAttribute UMMC_MaxMana::GetBaseAttribute() const
{
	return UAuraAttributeSet::GetIntelligenceAttribute();
}
float UMMC_MaxMana::CalculateBaseMagnitude_Implementation( const FGameplayEffectSpec& Spec ) const
{
	return Super::CalculateBaseMagnitude_Implementation( Spec );
}
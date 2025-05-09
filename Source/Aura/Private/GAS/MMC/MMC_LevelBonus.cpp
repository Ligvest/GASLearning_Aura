// Dovzhik Tolya

#include "GAS/MMC/MMC_LevelBonus.h"

#include "GAS/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_LevelBonus::UMMC_LevelBonus()
{
}

FGameplayAttribute UMMC_LevelBonus::GetBaseAttribute() const
{
	// Should be implemented in childrens
	return FGameplayAttribute();
}
void UMMC_LevelBonus::InitBaseAttributeDef()
{
	// The same parameters as in GE blueprints in Modifiers
	BaseAttributeDef.AttributeToCapture = GetBaseAttribute();
	BaseAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	BaseAttributeDef.bSnapshot = false;

	// Capture this attribute to use it's value later using GetCapturedAttributeMagnitude
	RelevantAttributesToCapture.Add( BaseAttributeDef );
}

float UMMC_LevelBonus::CalculateBaseMagnitude_Implementation( const FGameplayEffectSpec& Spec ) const
{
	// Init EvaluateParameters to pass it to GetCapturedAttributeMagnitude
	// It will allow to consider these tags in GE. Restrict Tags, etc. Maybe I'm wrong
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// Get Actor Level
	int ActorLevel = 0;
	UObject* EffectSource = Spec.GetContext().GetSourceObject();
	check( EffectSource );

	ICombatInterface* CombatInterface = Cast<ICombatInterface>( EffectSource );
	if ( CombatInterface )
	{
		ActorLevel = CombatInterface->GetActorLevel();
	}

	// Get BaseAttribute
	float BaseAttributeMagnitude = 0.f;
	GetCapturedAttributeMagnitude( BaseAttributeDef, Spec, EvaluateParameters, BaseAttributeMagnitude );

	const float ActorLevelBonus = CalcParams.ActorLevelCoefficient * ActorLevel;
	return ( CalcParams.PreMultiplyAdditiveValue + BaseAttributeMagnitude ) * CalcParams.Coefficient + CalcParams.PostMultiplyAdditiveValue + ActorLevelBonus;
}
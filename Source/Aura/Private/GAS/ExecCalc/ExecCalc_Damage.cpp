// Dovzhik Tolya

#include "GAS/ExecCalc/ExecCalc_Damage.h"

#include "GAS/AuraAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "GAS/AuraGasBpLibrary.h"
#include "Interaction/CombatInterface.h"

struct AuraDamageStatics
{
	// These macros are just a short way to init FGameplayEffectAttributeCaptureDefinition to capture an attribute
	// The long way I did in MMCs. E.g. UMMC_LevelBonus
	// Source ASC attributes
	DECLARE_ATTRIBUTE_CAPTUREDEF( ArmorPenetration );
	DECLARE_ATTRIBUTE_CAPTUREDEF( CriticalHitChance );
	DECLARE_ATTRIBUTE_CAPTUREDEF( CriticalHitDamage );
	// Target ASC attributes
	DECLARE_ATTRIBUTE_CAPTUREDEF( Armor );
	DECLARE_ATTRIBUTE_CAPTUREDEF( BlockChance );
	DECLARE_ATTRIBUTE_CAPTUREDEF( CriticalHitResistance );

	// Input Parameters:
	// AttributeSet class
	// Attribute name
	// From which ASC should be taken the attribute ( Source or Target )
	// bool: Snapshot or not
	AuraDamageStatics()
	{
		// Source ASC attributes
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, ArmorPenetration, Source, false );
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, CriticalHitChance, Source, false );
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, CriticalHitDamage, Source, false );
		// Target ASC attributes
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, Armor, Target, false );
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, BlockChance, Target, false );
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, CriticalHitResistance, Target, false );
	}
};

// Just making kinda singletone
static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	// Add the attribute to capture it in Execute_Implementation
	// Source ASC attributes
	RelevantAttributesToCapture.Add( DamageStatics().ArmorPenetrationDef );
	RelevantAttributesToCapture.Add( DamageStatics().CriticalHitChanceDef );
	RelevantAttributesToCapture.Add( DamageStatics().CriticalHitDamageDef );
	// Target ASC attributes
	RelevantAttributesToCapture.Add( DamageStatics().ArmorDef );
	RelevantAttributesToCapture.Add( DamageStatics().BlockChanceDef );
	RelevantAttributesToCapture.Add( DamageStatics().CriticalHitResistanceDef );
}

void UExecCalc_Damage::Execute_Implementation( const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput ) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	const AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	const ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>( SourceAvatar );
	const ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>( TargetAvatar );
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	UAuraCharacterClassInfoDA* CharacterClassInfo = UAuraGasBpLibrary::GetCharacterClassInfoDA( TargetAvatar );

	// We should pass these tags to AttemptCalculateCapturedAttributeMagnitude so it can take them in account to calculate correct value
	FAggregatorEvaluateParameters EvaluationParams;
	EvaluationParams.SourceTags = SourceTags;
	EvaluationParams.TargetTags = TargetTags;

	CalculationInfo CalcInfo;
	CalcInfo.SourceCombatInterface = SourceCombatInterface;
	CalcInfo.TargetCombatInterface = TargetCombatInterface;
	CalcInfo.ExecutionParams = &ExecutionParams;
	CalcInfo.EvaluationParams = &EvaluationParams;
	CalcInfo.CharacterClassInfo = CharacterClassInfo;
	if ( SourceCombatInterface )
	{
		CalcInfo.SourceActorLevel = SourceCombatInterface->GetActorLevel();
	}

	if ( TargetCombatInterface )
	{
		CalcInfo.TargetActorLevel = TargetCombatInterface->GetActorLevel();
	}

	// Captured Damage with Values_Damage tag from SetByCaller
	float Damage = Spec.GetSetByCallerMagnitude( GameplayTags.Values_Damage );
	ModifyDamageByBlockChance( Damage, CalcInfo );
	ModifyDamageByArmor( Damage, CalcInfo );
	ModifyDamageByCriticalHit( Damage, CalcInfo );

	// Add Damage to IncomingDamage attribute. IncomingDamage is used further to subtract Health in PostGameplayEffectExecute
	const FGameplayModifierEvaluatedData EvaluatedDataDamage( UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage );
	// Pass the EvaluatedData ( with Armor ) to the output
	OutExecutionOutput.AddOutputModifier( EvaluatedDataDamage );
}

void UExecCalc_Damage::ModifyDamageByBlockChance( float& OutDamage, const CalculationInfo& CalcInfo ) const
{
	// Captured BlockChance from Target ASC
	float TargetBlockChance = 0.f;
	CalcInfo.ExecutionParams->AttemptCalculateCapturedAttributeMagnitude( DamageStatics().BlockChanceDef, *CalcInfo.EvaluationParams, TargetBlockChance );
	float Odds = FMath::RandRange( 1.f, 100.f );

	// Captured Damage with Values_Damage tag from SetByCaller
	if ( TargetBlockChance >= Odds )
	{
		// Halve the damage if it was blocked
		OutDamage = OutDamage / 2;
	}
}
void UExecCalc_Damage::ModifyDamageByArmor( float& OutDamage, const CalculationInfo& CalcInfo ) const
{
	const UCurveTable* CalcCoefsCT = CalcInfo.CharacterClassInfo->CalculationCoefficientsCT;
	check( CalcCoefsCT );

	// Get necessary attributes
	float TargetArmor = 0.f;
	CalcInfo.ExecutionParams->AttemptCalculateCapturedAttributeMagnitude( DamageStatics().ArmorDef, *CalcInfo.EvaluationParams, TargetArmor );
	float SourceArmorPenetration = 0.f;
	CalcInfo.ExecutionParams->AttemptCalculateCapturedAttributeMagnitude( DamageStatics().ArmorPenetrationDef, *CalcInfo.EvaluationParams, SourceArmorPenetration );

	// Get necessary coefficients
	FRealCurve* ArmorCoefCurve = CalcCoefsCT->FindCurve( FName( "Armor" ), FString() );
	check( ArmorCoefCurve );
	const float ArmorCoef = ArmorCoefCurve->Eval( CalcInfo.TargetActorLevel );
	const float MaxEffectiveArmor = 100.f / ArmorCoef;  // The value of Armor to ignore 100% of Damage

	// Captured ArmorPenetration from Source ASC
	FRealCurve* ArmorPenetrationCoefCurve = CalcCoefsCT->FindCurve( FName( "ArmorPenetration" ), FString() );
	check( ArmorPenetrationCoefCurve );
	const float ArmorPenetrationCoef = ArmorPenetrationCoefCurve->Eval( CalcInfo.SourceActorLevel );
	const float MaxArmorPenetration = 100.f / ArmorPenetrationCoef;  // The value of ArmorPenetration to ignore 100% of Armor

	// Make calculations
	SourceArmorPenetration = FMath::Clamp( SourceArmorPenetration, 0.f, MaxArmorPenetration );
	// Ignore (SourceArmorPenetration * ArmorPenetrationCoef) percents of TargetArmor
	float EffectiveTargetArmor = TargetArmor * ( 100 - SourceArmorPenetration * ArmorPenetrationCoef ) / 100.f;
	EffectiveTargetArmor = FMath::Clamp( EffectiveTargetArmor, 0.f, MaxEffectiveArmor );
	// Subtract (EffectiveTargeArmor * ArmorCoef) percents of Damage
	OutDamage *= ( 100 - EffectiveTargetArmor * ArmorCoef ) / 100;
}

void UExecCalc_Damage::ModifyDamageByCriticalHit( float& OutDamage, const CalculationInfo& CalcInfo ) const
{
	const UCurveTable* CalcCoefsCT = CalcInfo.CharacterClassInfo->CalculationCoefficientsCT;
	check( CalcCoefsCT );

	// Get necessary attributes
	float SourceCriticalHitChance = 0.f;
	CalcInfo.ExecutionParams->AttemptCalculateCapturedAttributeMagnitude( DamageStatics().CriticalHitChanceDef, *CalcInfo.EvaluationParams, SourceCriticalHitChance );
	float TargetCriticalHitResistance = 0.f;
	CalcInfo.ExecutionParams->AttemptCalculateCapturedAttributeMagnitude( DamageStatics().CriticalHitResistanceDef, *CalcInfo.EvaluationParams, TargetCriticalHitResistance );
	float SourceCriticalHitDamage = 0.f;
	CalcInfo.ExecutionParams->AttemptCalculateCapturedAttributeMagnitude( DamageStatics().CriticalHitDamageDef, *CalcInfo.EvaluationParams, SourceCriticalHitDamage );

	// Get necessary coefficients
	FRealCurve* CriticalHitResistanceCoefCurve = CalcCoefsCT->FindCurve( FName( "CriticalHitResistance" ), FString() );
	check( CriticalHitResistanceCoefCurve );
	const float CriticalHitResistanceCoef = CriticalHitResistanceCoefCurve->Eval( CalcInfo.TargetActorLevel );

	// Make calculations
	float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoef;
	float Odds = FMath::RandRange( 1.f, 100.f );
	bool bCriticalHit = EffectiveCriticalHitChance >= Odds;
	if ( bCriticalHit )
	{
		OutDamage = OutDamage * 2 + SourceCriticalHitDamage;
	}
}

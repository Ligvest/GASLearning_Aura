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
	DECLARE_ATTRIBUTE_CAPTUREDEF( BlockChance );
	DECLARE_ATTRIBUTE_CAPTUREDEF( Armor );
	DECLARE_ATTRIBUTE_CAPTUREDEF( CriticalHitResistance );
	DECLARE_ATTRIBUTE_CAPTUREDEF( ArcaneResistance );
	DECLARE_ATTRIBUTE_CAPTUREDEF( FireResistance );
	DECLARE_ATTRIBUTE_CAPTUREDEF( LightningResistance );
	DECLARE_ATTRIBUTE_CAPTUREDEF( PhysicalResistance );

	AuraDamageStatics()
	{
		// Input Parameters:
		// AttributeSet class
		// Attribute name
		// From which ASC should be taken the attribute ( Source or Target )
		// bool: Snapshot or not

		// Source ASC attributes
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, ArmorPenetration, Source, false );
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, CriticalHitChance, Source, false );
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, CriticalHitDamage, Source, false );
		// Target ASC attributes
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, Armor, Target, false );
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, BlockChance, Target, false );
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, CriticalHitResistance, Target, false );

		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, ArcaneResistance, Target, false );
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, FireResistance, Target, false );
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, LightningResistance, Target, false );
		DEFINE_ATTRIBUTE_CAPTUREDEF( UAuraAttributeSet, PhysicalResistance, Target, false );
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

	RelevantAttributesToCapture.Add( DamageStatics().ArcaneResistanceDef );
	RelevantAttributesToCapture.Add( DamageStatics().FireResistanceDef );
	RelevantAttributesToCapture.Add( DamageStatics().LightningResistanceDef );
	RelevantAttributesToCapture.Add( DamageStatics().PhysicalResistanceDef );
}

void UExecCalc_Damage::Execute_Implementation( const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput ) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	const AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetEffectContext();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	const AuraDamageStatics& CapturedCombatAttributes = DamageStatics();

	// Not sure if this struct is necessary. Also moved it from ExecCalc_Damage constructor because
	// this ctor is called earlier than GameplayTags are initialized. So this is a workaround. Also the same way Stephan did this
	// The Struct is static so there is no need to use anything aside of raw pointers for CaptureDefinition
	TMap<FGameplayTag, const FGameplayEffectAttributeCaptureDefinition*> TagToAttributeCapture;
	TagToAttributeCapture.Add( GameplayTags.Attributes_Resistance_Arcane, &CapturedCombatAttributes.ArcaneResistanceDef );
	TagToAttributeCapture.Add( GameplayTags.Attributes_Resistance_Fire, &CapturedCombatAttributes.FireResistanceDef );
	TagToAttributeCapture.Add( GameplayTags.Attributes_Resistance_Lightning, &CapturedCombatAttributes.LightningResistanceDef );
	TagToAttributeCapture.Add( GameplayTags.Attributes_Resistance_Physical, &CapturedCombatAttributes.PhysicalResistanceDef );

	// We should pass these tags to AttemptCalculateCapturedAttributeMagnitude so it can take them in account to calculate correct value
	FAggregatorEvaluateParameters EvaluationParams;
	EvaluationParams.SourceTags = SourceTags;
	EvaluationParams.TargetTags = TargetTags;

	CalculationInfo CalcInfo;
	CalcInfo.SourceCombatInterface = Cast<ICombatInterface>( SourceAvatar );
	CalcInfo.TargetCombatInterface = Cast<ICombatInterface>( TargetAvatar );
	CalcInfo.ExecutionParams = &ExecutionParams;
	CalcInfo.EvaluationParams = &EvaluationParams;
	CalcInfo.CharacterClassInfo = UAuraGasBpLibrary::GetCharacterClassInfoDA( TargetAvatar );
	if ( CalcInfo.SourceCombatInterface )
	{
		CalcInfo.SourceActorLevel = CalcInfo.SourceCombatInterface->GetCharacterLevel();
	}

	if ( CalcInfo.TargetCombatInterface )
	{
		CalcInfo.TargetActorLevel = CalcInfo.TargetCombatInterface->GetCharacterLevel();
	}

	// Captured Damage with Values_Damage tag from SetByCaller
	float CombinedDamage = 0.f;
	for ( const auto& [DamageTypeTag, ResTypeTag] : GameplayTags.DamageTypeToDamageRes )
	{
		// Read each type of damage passed to SetByCaller function
		// If no such type the function returns 0
		float DamageValue = Spec.GetSetByCallerMagnitude( DamageTypeTag, false, -1.f );
		if ( DamageValue < 0.f )
		{
			continue;
		}

		// Get resistance value from captured attributes for the DamageTypeTag
		float TargetDamageTypeResistance = 0.f;
		const FGameplayEffectAttributeCaptureDefinition* const* ResistanceAttributeCapturePtr = TagToAttributeCapture.Find( ResTypeTag );
		check( ResistanceAttributeCapturePtr );
		const FGameplayEffectAttributeCaptureDefinition& ResistanceAttributeDef = **ResistanceAttributeCapturePtr;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude( ResistanceAttributeDef, EvaluationParams, TargetDamageTypeResistance );

		const float SourceDebuffChance = Spec.GetSetByCallerMagnitude( GameplayTags.Debuff_Chance, false, -1.f );
		bool bDebuffSucceeded = CheckOdds( SourceDebuffChance, TargetDamageTypeResistance );
		if ( bDebuffSucceeded )
		{
			ApplyDebuff( Spec, DamageTypeTag );
		}

		TargetDamageTypeResistance = std::clamp( TargetDamageTypeResistance, 0.f, 100.f );

		// "100.f - ResistanceValue" = Percent of damage not ignored
		CombinedDamage += DamageValue * ( 100.f - TargetDamageTypeResistance ) / 100.f;
	}

	if ( CombinedDamage > 0.f )
	{
		float KnockbackChance = Spec.GetSetByCallerMagnitude( GameplayTags.Knockback_Chance, false, -1.f );
		bool bKnockbackSucceeded = CheckOdds( KnockbackChance, 0.f );
		if ( bKnockbackSucceeded )
		{
			// Do Knockback
		}
	}

	// Calculate Block affection
	bool bIsBlockedHit = false;
	ModifyDamageByBlockChance( CombinedDamage, CalcInfo, bIsBlockedHit );
	UAuraGasBpLibrary::SetIsBlockedHitFromEffectContext( EffectContextHandle, bIsBlockedHit );

	// Calculate Armor affection
	ModifyDamageByArmor( CombinedDamage, CalcInfo );

	// Calculate CriticalHit affection
	bool bIsCriticalHit = false;
	ModifyDamageByCriticalHit( CombinedDamage, CalcInfo, bIsCriticalHit );
	UAuraGasBpLibrary::SetIsCriticalHitInEffectContext( EffectContextHandle, bIsCriticalHit );

	// Add Damage to IncomingDamage attribute. IncomingDamage is used further to subtract Health in PostGameplayEffectExecute
	const FGameplayModifierEvaluatedData EvaluatedDataDamage( UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, CombinedDamage );
	// Pass the EvaluatedData ( with Armor ) to the output
	OutExecutionOutput.AddOutputModifier( EvaluatedDataDamage );
}

void UExecCalc_Damage::ModifyDamageByBlockChance( float& OutDamage, const CalculationInfo& CalcInfo, bool& bOutBlockedHit ) const
{
	// Captured BlockChance from Target ASC
	float TargetBlockChance = 0.f;
	CalcInfo.ExecutionParams->AttemptCalculateCapturedAttributeMagnitude( DamageStatics().BlockChanceDef, *CalcInfo.EvaluationParams, TargetBlockChance );
	bOutBlockedHit = CheckOdds( TargetBlockChance, 0.f );
	if ( bOutBlockedHit )
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

void UExecCalc_Damage::ModifyDamageByCriticalHit( float& OutDamage, const CalculationInfo& CalcInfo, bool& bOutCriticalHit ) const
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
	bOutCriticalHit = EffectiveCriticalHitChance >= Odds;
	if ( bOutCriticalHit )
	{
		OutDamage = OutDamage * 2 + SourceCriticalHitDamage;
	}
}

void UExecCalc_Damage::ApplyDebuff( const FGameplayEffectSpec& Spec, FGameplayTag DamageTypeTag ) const
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	FGameplayEffectContextHandle ContextHandle = Spec.GetContext();

	const float DebuffDamage = Spec.GetSetByCallerMagnitude( GameplayTags.Debuff_Damage, false, -1.f );
	const float DebuffDuration = Spec.GetSetByCallerMagnitude( GameplayTags.Debuff_Duration, false, -1.f );
	const float DebuffFrequency = Spec.GetSetByCallerMagnitude( GameplayTags.Debuff_Frequency, false, -1.f );

	// This is one way to pass data from ExecCalc to AttributeSet PostGameplayEffectExecute
	// But this is a stupid way as we still have access to GameplayEffectSpec from PostGameplayEffectExecute function
	// And from this Spec we easily can access all SetByCallerMagnitude data which already has all this shit
	// Also we can use SetByCallerMagnitude to pass a bool as a number.
	// So for learning purposes this is nice to try to make your custom EffectContext but in this situation you mustn't use it
	UAuraGasBpLibrary::SetIsDebuffSucceededInEffectContext( ContextHandle, true );
	UAuraGasBpLibrary::SetDamageTypeTagInEffectContext( ContextHandle, DamageTypeTag );
	UAuraGasBpLibrary::SetDebuffDamageInEffectContext( ContextHandle, DebuffDamage );
	UAuraGasBpLibrary::SetDebuffDurationInEffectContext( ContextHandle, DebuffDuration );
	UAuraGasBpLibrary::SetDebuffFrequencyInEffectContext( ContextHandle, DebuffFrequency );
}

bool UExecCalc_Damage::CheckOdds( const float Chance, float Resistance ) const
{
	Resistance = FMath::Max<float>( Resistance, 0.f );
	const float EffectiveDebuffChance = Chance * ( 100 - Resistance ) / 100.f;
	return EffectiveDebuffChance >= FMath::RandRange( 1, 100 );
}

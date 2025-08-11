// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecCalc_Damage.generated.h"

class ICombatInterface;
class UAuraCharacterClassInfoDA;

struct CalculationInfo
{
	const ICombatInterface* SourceCombatInterface;
	const ICombatInterface* TargetCombatInterface;
	// const FAuraGameplayTags& GameplayTags;
	UAuraCharacterClassInfoDA* CharacterClassInfo;
	const FGameplayEffectCustomExecutionParameters* ExecutionParams;
	const FAggregatorEvaluateParameters* EvaluationParams;
	int SourceActorLevel = 0;
	int TargetActorLevel = 0;
};

/**
 *
 */
UCLASS()
class AURA_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UExecCalc_Damage();

	virtual void Execute_Implementation( const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput ) const override;

private:
	void ModifyDamageByBlockChance( float& OutDamage, const CalculationInfo& CalcInfo, bool& bOutBlockedHit ) const;
	void ModifyDamageByArmor( float& OutDamage, const CalculationInfo& CalcInfo ) const;
	void ModifyDamageByCriticalHit( float& OutDamage, const CalculationInfo& CalcInfo, bool& bOutCriticalHit ) const;
	void ApplyDebuff( const FGameplayEffectSpec& Spec, FGameplayTag DamageTypeTag ) const;
	bool CheckOdds( const float Chance, float Resistance ) const;
};

// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "GAS/Abilities/AuraGameplayAbility.h"
#include "AuraDamageGameplayAbility.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

private:
	UFUNCTION( BlueprintCallable )
	void CauseDamage( AActor* TargetActor );

protected:
	UFUNCTION( BlueprintPure )
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults( AActor* TargetActor = nullptr ) const;

public:
	UPROPERTY( EditDefaultsOnly, Category = "Damage" )
	FGameplayTag DamageTypeTag;

	UPROPERTY( EditDefaultsOnly, Category = "Damage" )
	FScalableFloat DamageScalableFloat;

	UPROPERTY( EditDefaultsOnly, Category = "Damage" )
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	// Debuffs info to pass using SetByCallerMagnitude
	UPROPERTY( EditDefaultsOnly, Category = "Damage" )
	float DebuffChance = 20.f;

	UPROPERTY( EditDefaultsOnly, Category = "Damage" )
	float DebuffDamage = 5.f;

	UPROPERTY( EditDefaultsOnly, Category = "Damage" )
	float DebuffDuration = 5.f;

	UPROPERTY( EditDefaultsOnly, Category = "Damage" )
	float DebuffFrequency = 1.f;

	UPROPERTY( EditDefaultsOnly, Category = "Damage" )
	float KnockbackChance = 20.f;

	UPROPERTY( EditDefaultsOnly, Category = "Damage" )
	float KnockbackMagnitude = 500.f;

	UPROPERTY( EditDefaultsOnly, Category = "Damage" )
	float DeathImpulseMagnitude = 60.f;
};

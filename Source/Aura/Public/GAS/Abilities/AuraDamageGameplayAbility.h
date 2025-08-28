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
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults( AActor* TargetActor = nullptr, FVector InRadialDamageOrigin = FVector::ZeroVector, bool bOverrideKnockbackDirection = false,
	                                                             FVector KnockbackDirectionOverride = FVector::ZeroVector, bool bOverrideDeathImpulse = false,
	                                                             FVector DeathImpulseDirectionOverride = FVector::ZeroVector, bool bOverridePitch = false, float PitchOverride = 0.f ) const;

	UFUNCTION( BlueprintPure )
	float GetDamageAtLevel() const;

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

	UPROPERTY( EditDefaultsOnly, Category = "Damage" )
	bool bIsRadialDamage = false;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Damage" )
	float RadialDamageInnerRadius = 0.f;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Damage" )
	float RadialDamageOuterRadius = 0.f;
};

// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/AuraGameplayAbility.h"
#include "AuraDamageGameplayAbility.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

	UFUNCTION( BlueprintCallable )
	void CauseDamage( AActor* TargetActor );

public:
	UPROPERTY( EditDefaultsOnly, Category = "Damage" )
	TMap<FGameplayTag, FScalableFloat> DamageTypeTagToScalableFloat;

	UPROPERTY( EditDefaultsOnly, Category = "Damage" )
	TSubclassOf<UGameplayEffect> DamageEffectClass;
};

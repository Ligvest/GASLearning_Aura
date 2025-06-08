// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/AuraGameplayAbility.h"
#include "AuraProjectileSpell.generated.h"

class AAuraProjectile;

/**
 *
 */
UCLASS()
class AURA_API UAuraProjectileSpell : public UAuraGameplayAbility
{
	GENERATED_BODY()

	virtual void ActivateAbility( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	                              const FGameplayEventData* TriggerEventData ) override;

	UPROPERTY( EditDefaultsOnly, Category = "Projectile" )
	TSubclassOf<AAuraProjectile> ProjectileClass;
};

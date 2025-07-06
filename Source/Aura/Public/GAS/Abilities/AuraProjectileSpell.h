// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "AuraDamageGameplayAbility.h"
#include "AuraProjectileSpell.generated.h"

class AAuraProjectile;

/**
 *
 */
UCLASS()
class AURA_API UAuraProjectileSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

	virtual void ActivateAbility( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	                              const FGameplayEventData* TriggerEventData ) override;

	UFUNCTION( BlueprintCallable, Category = "Projectile" )
	void SpawnProjectile( FVector TargetLocation );

	UPROPERTY( EditDefaultsOnly, Category = "Projectile" )
	TSubclassOf<AAuraProjectile> ProjectileClass;
};

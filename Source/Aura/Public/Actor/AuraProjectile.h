// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "AuraProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class AURA_API AAuraProjectile : public AActor
{
	GENERATED_BODY()

public:
	AAuraProjectile();

	void SetImpactEffectParams( FDamageEffectParams&& InImpactEffectParams ) { ImpactEffectParams = MoveTemp( InImpactEffectParams ); };

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	UFUNCTION()
	void OnCollisionSphereOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult );

	void PlayImpactEffects() const;

	UPROPERTY( VisibleAnywhere, Category = "Collision" )
	TObjectPtr<USphereComponent> CollisionSphere;

	UPROPERTY( VisibleAnywhere, Category = "Movement" )
	TObjectPtr<UProjectileMovementComponent> MovementComponent;

	UPROPERTY( EditDefaultsOnly, Category = "Fly" )
	TObjectPtr<USoundBase> FlySound;

	UPROPERTY()
	TObjectPtr<UAudioComponent> FlySoundComponent;

	float LifeSpan = 5.f;
	bool bImpactHappened = false;

	UPROPERTY( EditDefaultsOnly, Category = "Impact" )
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY( EditDefaultsOnly, Category = "Impact" )
	TObjectPtr<USoundBase> ImpactSound;

	FDamageEffectParams ImpactEffectParams;
};

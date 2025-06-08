// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class AURA_API AAuraProjectile : public AActor
{
	GENERATED_BODY()

public:
	AAuraProjectile();

	UFUNCTION()
	void OnCollisionSphereOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult );

protected:
	virtual void BeginPlay() override;

	UPROPERTY( VisibleAnywhere, Category = "Collision" )
	TObjectPtr<USphereComponent> CollisionSphere;

	UPROPERTY( VisibleAnywhere, Category = "Movement" )
	TObjectPtr<UProjectileMovementComponent> MovementComponent;
};

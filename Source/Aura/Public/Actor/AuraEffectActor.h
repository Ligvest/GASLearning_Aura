// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"

// Forward declarations:
class USphereComponent;

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()

public:
	AAuraEffectActor();

	UFUNCTION()
	void OnBeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult );
	UFUNCTION()
	void OnEndOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex );

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY( VisibleAnywhere )
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY( VisibleAnywhere )
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
};

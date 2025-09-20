// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/HighlightActorInterface.h"
#include "Interaction/SaveInterface.h"
#include "Checkpoint.generated.h"

enum class EHighlightActorType : uint8;
class USphereComponent;
/**
 *
 */
UCLASS()
class AURA_API ACheckpoint : public APlayerStart, public ISaveInterface, public IHighlightActorInterface
{
	GENERATED_BODY()
public:
	ACheckpoint( const FObjectInitializer& ObjectInitializer );

	/* Save Interface */
	virtual bool ShouldLoadTransform_Implementation() override { return false; };
	virtual void LoadActor_Implementation() override;
	/* end Save Interface */

	//~ Begin of IHightlightActorInterface
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	virtual void SetMoveToLocation_Implementation( FVector& OutDestination ) override;
	//~ End of IHightlightActorInterface

protected:
	UFUNCTION()
	virtual void OnSphereOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult );

	virtual void BeginPlay() override;

	UFUNCTION( BlueprintImplementableEvent )
	void CheckpointReached( UMaterialInstanceDynamic* DynamicMaterialInstance );

	void HandleGlowEffects();

protected:
	UPROPERTY( VisibleAnywhere )
	TObjectPtr<UStaticMeshComponent> CheckpointMesh;

	UPROPERTY( VisibleAnywhere )
	TObjectPtr<USceneComponent> MoveToComponent;

	UPROPERTY( VisibleAnywhere )
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY( BlueprintReadOnly, SaveGame )
	bool bReached = false;

private:
	UPROPERTY( EditAnywhere, Category = Interaction )
	EHighlightActorType HighlightActorType;
};

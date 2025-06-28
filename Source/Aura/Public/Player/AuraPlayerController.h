// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class UDamageTextComponent;
class USplineComponent;
// Forward declaration
class IHighlightActorInterface;
class UInputMappingContext;
class UInputAction;
class UAuraInputConfig;
class UAuraAbilitySystemComponent;
struct FGameplayTag;
struct FInputActionValue;

/**
 *
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AAuraPlayerController();

	UFUNCTION( Client, Reliable )
	void ShowDamageNumber( float Damage, ACharacter* TargetCharacter, bool bIsBlockedHit, bool bIsCriticalHit ) const;

protected:
	virtual void PlayerTick( float DeltaSeconds ) override;
	virtual void BeginPlay() override;

	// Input
	void MoveWithButtons( const FInputActionValue& InputActionValue );
	void MoveWithCursor();
	void GeneratePathToPoint( FVector TargetPoint );
	void ShiftPressed() { bShiftPressed = true; }
	void ShiftReleased() { bShiftPressed = false; }
	void AutoMoveAlongMovementSpline();
	void CursorTrace();
	void UpdateHightlightActor();
	virtual void SetupInputComponent() override;
	void AbilityInputTagPressed( FGameplayTag InputTag );
	void AbilityInputTagReleased( FGameplayTag InputTag );
	void AbilityInputTagHeld( FGameplayTag InputTag );

private:
	void BeginPlayClientOnly();
	UAuraAbilitySystemComponent* GetASC();

	UPROPERTY( EditAnywhere, Category = "Input" )
	TObjectPtr<UInputMappingContext> IMC;

	UPROPERTY( EditAnywhere, Category = "Input" )
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY( EditAnywhere, Category = "Input" )
	TObjectPtr<UInputAction> ShiftAction;

	UPROPERTY( EditDefaultsOnly, Category = "Input" )
	TObjectPtr<UAuraInputConfig> InputConfig;

	TScriptInterface<IHighlightActorInterface> CurrentActorUnderCursorToHighlight;
	TScriptInterface<IHighlightActorInterface> LastActorUnderCursorToHighlight;

	UAuraAbilitySystemComponent* ASC;

	// Input
	UPROPERTY( VisibleAnywhere, Category = "Input" )
	TObjectPtr<USplineComponent> AutoMoveSpline;
	AActor* ActorUnderCursor;
	FVector LastCursorTraceImpactPoint;

	FVector AutoMoveDestinationPoint;
	FVector AutoMoveDirection = FVector::ZeroVector;
	bool bAutoMove = false;
	float AutoMoveDisableDistanceThreshold = 50.f;
	bool bTargeting = false;
	float FollowTime = 0.f;
	bool bShiftPressed = false;

	UPROPERTY( EditDefaultsOnly, Category = "Input" )
	float HoldButtonThreshold = 0.3;

	UPROPERTY( EditDefaultsOnly, Category = "UI" )
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

	// Debug
public:
	// Debug:
	void DrawSplineDebug( USplineComponent* Spline, FColor Color = FColor::Red, float Step = 10.0f );
};

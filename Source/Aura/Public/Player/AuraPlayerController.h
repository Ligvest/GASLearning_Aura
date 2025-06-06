// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

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

protected:
	virtual void PlayerTick( float DeltaSeconds ) override;
	virtual void BeginPlay() override;

	// Input
	void Move( const FInputActionValue& InputActionValue );
	void CursorTrace();
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

	UPROPERTY( EditDefaultsOnly, Category = "Input" )
	TObjectPtr<UAuraInputConfig> InputConfig;

	TScriptInterface<IHighlightActorInterface> ThisActorUnderCursorToHighlight;
	TScriptInterface<IHighlightActorInterface> LastActorUnderCursorToHighlight;

	UAuraAbilitySystemComponent* ASC;
};

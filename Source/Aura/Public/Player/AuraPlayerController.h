// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

// Forward declaration
class IHighlightActorInterface;
class UInputMappingContext;
class UInputAction;
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
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void Move( const FInputActionValue& InputActionValue );
	void CursorTrace();
	virtual void PlayerTick( float DeltaSeconds ) override;

private:
	UPROPERTY( EditAnywhere, Category = "Input" )
	TObjectPtr<UInputMappingContext> IMC;

	UPROPERTY( EditAnywhere, Category = "Input" )
	TObjectPtr<UInputAction> MoveAction;

	TScriptInterface<IHighlightActorInterface> ThisActorUnderCursorToHighlight;
	TScriptInterface<IHighlightActorInterface> LastActorUnderCursorToHighlight;
};

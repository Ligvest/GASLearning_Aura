// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "AuraAICharacter.h"
#include "AuraEnemyCharacter.generated.h"

class UWidgetComponent;
/**
 *
 */
UCLASS()
class AURA_API AAuraEnemyCharacter : public AAuraAICharacter
{
public:
	AAuraEnemyCharacter();
	virtual void BeginPlay() override;

	UFUNCTION( BlueprintPure )
	UAuraEnemyOverlayWC* GetOverlayWC() const;

protected:
	UPROPERTY()
	TObjectPtr<UAuraEnemyOverlayWC> FloatingWC;

	UPROPERTY( EditDefaultsOnly )
	TSubclassOf<UAuraEnemyOverlayWC> FloatingWcClass;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly )
	TObjectPtr<UWidgetComponent> FloatingWidget;

	UFUNCTION( BlueprintImplementableEvent )
	void SetupFloatingWidget();

	void InitFloatingWC();

	GENERATED_BODY()
};

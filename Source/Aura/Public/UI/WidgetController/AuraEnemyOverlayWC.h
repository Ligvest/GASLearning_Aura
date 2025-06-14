// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraEnemyOverlayWC.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UAuraEnemyOverlayWC : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	UFUNCTION( BlueprintCallable )
	virtual void BroadcastInitialValues() const override;

	UPROPERTY( BlueprintAssignable )
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY( BlueprintAssignable )
	FOnAttributeChangedSignature OnMaxHealthChanged;

protected:
	virtual void BindCallbacksToAttributeChanges() const override;
};

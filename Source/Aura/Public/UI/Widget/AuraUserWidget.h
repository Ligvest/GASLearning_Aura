// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

class UAuraWidgetController;
/**
 *
 */
UCLASS()
class AURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY( BlueprintReadOnly )
	TObjectPtr<UObject> WidgetController;

	UFUNCTION()
	void SetWidgetController( UObject* InWidgetController );

protected:
	// Called when WidgetController is set
	UFUNCTION( BlueprintImplementableEvent )
	void WidgetControllerSet();
};

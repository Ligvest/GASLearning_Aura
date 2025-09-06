// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE( FOnWidgetClosed );

class UAuraWidgetController;
class UButton;
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

	UFUNCTION( BlueprintCallable )
	void SetWidgetController( UObject* InWidgetController );

	UFUNCTION( BlueprintCallable )
	void Open( UUserWidget* InParentWidget, UButton* OpenButton );
	UFUNCTION( BlueprintCallable )
	void Close( bool ReturnInputToCharacter = true );

protected:
	// Called when WidgetController is set
	UFUNCTION( BlueprintImplementableEvent )
	void WidgetControllerSet();

	UPROPERTY()
	TObjectPtr<UButton> ParentButton;

	UPROPERTY()
	TObjectPtr<UUserWidget> ParentWidget;

	UPROPERTY( BlueprintAssignable )
	FOnWidgetClosed OnWidgetClosed;
};

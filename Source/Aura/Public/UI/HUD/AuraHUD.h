// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

class UAuraHUDWidgetController;
/**
 *
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()
public:
	void InitHUDWidget();

protected:
	UPROPERTY()
	TObjectPtr<UAuraHUDWidgetController> HUDWidgetController;

	UPROPERTY( EditAnywhere )
	TSubclassOf<UUserWidget> HUDWidgetClass;

	// In case we would want a BP widget controller
	UPROPERTY( EditAnywhere )
	TSubclassOf<UAuraHUDWidgetController> HUDWidgetControllerClass;

private:
	void InitWidgetController();
};

// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraHUD.generated.h"

// Forward declarations
class UAuraHUDWidgetController;
class UAuraAttributeWindowWC;
class UAuraSpellsWindowWC;

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
	// HUD Widget
	UPROPERTY( EditAnywhere )
	TSubclassOf<UUserWidget> HUDWidgetClass;

	// HUD WC
	UPROPERTY()
	TObjectPtr<UAuraHUDWidgetController> HUDWidgetController;

	UPROPERTY( EditAnywhere )
	TSubclassOf<UAuraHUDWidgetController> HUDWidgetControllerClass;

	// Attribute Window WC
	UPROPERTY()
	TObjectPtr<UAuraAttributeWindowWC> AttributeWindowWC;

	UPROPERTY( EditAnywhere )
	TSubclassOf<UAuraAttributeWindowWC> AttributeWindowWcClass;

	// Spell Window WC
	UPROPERTY()
	TObjectPtr<UAuraSpellsWindowWC> SpellsWindowWC;

	UPROPERTY( EditAnywhere )
	TSubclassOf<UAuraSpellsWindowWC> SpellsWindowWcClass;

public:
	UAuraHUDWidgetController* TryGetHudWC( const FWidgetControllerParams& Params );
	UAuraAttributeWindowWC* TryGetAttributeWindowWC( const FWidgetControllerParams& Params );
	UAuraSpellsWindowWC* TryGetSpellsWindowWC( const FWidgetControllerParams& Params );
};

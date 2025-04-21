// Dovzhik Tolya

#include "UI/HUD/AuraHUD.h"

#include "Blueprint/UserWidget.h"
#include "Player/AuraPlayerState.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/AuraHUDWidgetController.h"

void AAuraHUD::InitHUDWidget()
{
	// In our case it's the best to call the initializtion in OnRep_PlayerState function.
	// On at this point we will know that PlayerState is replicated. PlayerController is local, so it should be good too
	// AbilitySystemComponent and AttributeSet are set in PlayerSet so they should be good too

	// Create and init HUDWidgetController
	InitWidgetController();

	// Create and init HUDWidget
	checkf( HUDWidgetClass, TEXT( "The variable is not set in blueprints" ) );
	UAuraUserWidget* HUDWidget = CreateWidget<UAuraUserWidget>( GetWorld(), HUDWidgetClass, "HUDWidget" );
	HUDWidget->SetWidgetController( HUDWidgetController );
	HUDWidgetController->BroadcastInitialValues();
	HUDWidget->AddToViewport();
}
void AAuraHUD::InitWidgetController()
{
	// Init Params
	FWidgetControllerParams WidgetControllerParams;
	// PlayerController
	WidgetControllerParams.PlayerController = GetOwningPlayerController();
	check( WidgetControllerParams.PlayerController );
	// PlayerState
	WidgetControllerParams.PlayerState = WidgetControllerParams.PlayerController->PlayerState;
	check( WidgetControllerParams.PlayerState );
	// AbilitySystemComponent
	const AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>( WidgetControllerParams.PlayerState );
	WidgetControllerParams.AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	check( WidgetControllerParams.AbilitySystemComponent );
	// AttributeSet
	WidgetControllerParams.AttributeSet = AuraPlayerState->GetAttributeSet();
	check( HUDWidgetControllerClass );
	HUDWidgetController = NewObject<UAuraHUDWidgetController>( this, HUDWidgetControllerClass );
	HUDWidgetController->SetWidgetControllerParams( WidgetControllerParams );
}
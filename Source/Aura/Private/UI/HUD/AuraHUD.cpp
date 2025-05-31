// Dovzhik Tolya

#include "UI/HUD/AuraHUD.h"

#include "Blueprint/UserWidget.h"
#include "GAS/AuraGasBpLibrary.h"
#include "Player/AuraPlayerState.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/AuraAttributeWindowWC.h"
#include "UI/WidgetController/AuraHUDWidgetController.h"

void AAuraHUD::InitHUDWidget()
{
	// In our case it's the best to call the initializtion in OnRep_PlayerState function.
	// On at this point we will know that PlayerState is replicated. PlayerController is local, so it should be good too
	// AbilitySystemComponent and AttributeSet are set in PlayerSet so they should be good too

	// Create and init HUDWidgetController
	// TODO: I think this shouldnt be here. This is very stupid but for now i'll leave it here
	UAuraGasBpLibrary::GetHudWC( this );

	// Create and init HUDWidget
	checkf( HUDWidgetClass, TEXT( "The variable is not set in blueprints" ) );
	UAuraUserWidget* HUDWidget = CreateWidget<UAuraUserWidget>( GetWorld(), HUDWidgetClass, "HUDWidget" );
	HUDWidget->SetWidgetController( HUDWidgetController );
	HUDWidgetController->BroadcastInitialValues();
	HUDWidget->AddToViewport();
}
UAuraHUDWidgetController* AAuraHUD::TryGetHudWC( const FWidgetControllerParams& Params )
{
	if ( !HUDWidgetController )
	{
		HUDWidgetController = NewObject<UAuraHUDWidgetController>( this, HUDWidgetControllerClass );
		HUDWidgetController->SetWidgetControllerParams( Params );
	}
	return HUDWidgetController;
}
UAuraAttributeWindowWC* AAuraHUD::TryGetAttributeWindowWC( const FWidgetControllerParams& Params )
{
	if ( !AttributeWindowWC )
	{
		AttributeWindowWC = NewObject<UAuraAttributeWindowWC>( this, AttributeWindowWcClass );
		AttributeWindowWC->SetWidgetControllerParams( Params );
	}
	return AttributeWindowWC;
}
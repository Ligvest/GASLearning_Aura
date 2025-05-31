// Dovzhik Tolya

#include "GAS/AuraGasBpLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

// TODO: these functions return initialize and return WC. It's better to remove initialization from here in case
// we need to get the WC many time ( so that we don't initialize each time the parameters )
UAuraHUDWidgetController* UAuraGasBpLibrary::GetHudWC( const UObject* WorldContext )
{
	// Init Params
	FWidgetControllerParams WidgetControllerParams( GetWidgetControllerParams( WorldContext ) );

	// Get HUD
	AAuraHUD* HUD = WidgetControllerParams.PlayerController->GetHUD<AAuraHUD>();
	check( HUD );

	return HUD->TryGetHudWC( WidgetControllerParams );
}

UAuraAttributeWindowWC* UAuraGasBpLibrary::GetAttributeWindowWC( const UObject* WorldContext )
{
	// Init Params
	FWidgetControllerParams WidgetControllerParams( GetWidgetControllerParams( WorldContext ) );

	// Get HUD
	AAuraHUD* HUD = WidgetControllerParams.PlayerController->GetHUD<AAuraHUD>();
	check( HUD );

	return HUD->TryGetAttributeWindowWC( WidgetControllerParams );
}

// I believe this function should be called after PlayerState replicated and is up to date
FWidgetControllerParams UAuraGasBpLibrary::GetWidgetControllerParams( const UObject* WorldContext )
{
	// I believe this function should be called after PlayerState replicated and is up to date
	// Init Params
	FWidgetControllerParams WidgetControllerParams;
	// PlayerController
	WidgetControllerParams.PlayerController = UGameplayStatics::GetPlayerController( WorldContext, 0 );
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

	return WidgetControllerParams;
}
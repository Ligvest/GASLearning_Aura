// Dovzhik Tolya

#include "UI/WidgetController/AuraWidgetController.h"

void UAuraWidgetController::SetWidgetControllerParams( const FWidgetControllerParams& WidgetControllerParams )
{
	PlayerState = WidgetControllerParams.PlayerState;
	PlayerController = WidgetControllerParams.PlayerController;
	AbilitySystemComponent = WidgetControllerParams.AbilitySystemComponent;
	AttributeSet = WidgetControllerParams.AttributeSet;
	BindCallbacksToAttributeChanges();
}
void UAuraWidgetController::BroadcastInitialValues() const
{
	checkf( false, TEXT( "Must be implemented in children" ) )
}
void UAuraWidgetController::BindCallbacksToAttributeChanges() const
{
	checkf( false, TEXT( "Must be implemented in children" ) )
}
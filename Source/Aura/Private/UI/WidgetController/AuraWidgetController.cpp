// Dovzhik Tolya

#include "UI/WidgetController/AuraWidgetController.h"

#include "GAS/AuraAbilitySystemComponent.h"
#include "GAS/AuraAttributeSet.h"
#include "GAS/Data/AuraAbilityInfo_DA.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"

void UAuraWidgetController::SetWidgetControllerParams( const FWidgetControllerParams& WidgetControllerParams )
{
	PlayerState = WidgetControllerParams.PlayerState;
	PlayerController = WidgetControllerParams.PlayerController;
	AbilitySystemComponent = WidgetControllerParams.AbilitySystemComponent;
	AttributeSet = WidgetControllerParams.AttributeSet;
	BindCallbacksToAttributeChanges();
}
void UAuraWidgetController::BroadcastInitialValues()
{
	checkf( false, TEXT( "Must be implemented in children" ) )
}
void UAuraWidgetController::BroadcastAbilityInfo()
{
	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda(
	    [this]( const FGameplayAbilitySpec& AbilitySpec )
	    {
		    check( GetAuraASC() );
		    FGameplayTag AbilityTag = AuraAbilitySystemComponent->GetAbilityTagFromSpec( AbilitySpec );
		    FAuraAbilityInfo AbilityInfo = AbilityInfoDataAsset->FindAbilityInfoForTag( AbilityTag );
		    AbilityInfo.InputTag = AuraAbilitySystemComponent->GetInputTagFromSpec( AbilitySpec );
		    AbilityInfo.AbilityStatusTag = AuraAbilitySystemComponent->GetStatusTagFromSpec( AbilitySpec );
		    AbilityInfoDelegate.Broadcast( AbilityInfo );
	    } );

	GetAuraASC()->ForEachAbility( BroadcastDelegate );
}

void UAuraWidgetController::BindCallbacksToAttributeChanges(){ checkf( false, TEXT( "Must be implemented in children" ) ) }

AAuraPlayerController* UAuraWidgetController::GetAuraPC()
{
	if ( AuraPlayerController == nullptr )
	{
		AuraPlayerController = Cast<AAuraPlayerController>( PlayerController );
	}
	return AuraPlayerController;
}

AAuraPlayerState* UAuraWidgetController::GetAuraPS()
{
	if ( AuraPlayerState == nullptr )
	{
		AuraPlayerState = Cast<AAuraPlayerState>( PlayerState );
	}
	return AuraPlayerState;
}

UAuraAbilitySystemComponent* UAuraWidgetController::GetAuraASC()
{
	if ( AuraAbilitySystemComponent == nullptr )
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>( AbilitySystemComponent );
	}
	return AuraAbilitySystemComponent;
}

UAuraAttributeSet* UAuraWidgetController::GetAuraAS()
{
	if ( AuraAttributeSet == nullptr )
	{
		AuraAttributeSet = Cast<UAuraAttributeSet>( AttributeSet );
	}
	return AuraAttributeSet;
}

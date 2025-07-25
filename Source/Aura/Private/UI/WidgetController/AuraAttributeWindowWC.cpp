// Dovzhik Tolya

#include "UI/WidgetController/AuraAttributeWindowWC.h"

#include "AuraGameplayTags.h"
#include "GAS/AuraAbilitySystemComponent.h"
#include "GAS/AuraAttributeSet.h"
#include "GAS/Data/AuraAttributeInfoDataAsset.h"
#include "Player/AuraPlayerState.h"

void UAuraAttributeWindowWC::BindCallbacksToAttributeChanges() const
{
	const UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>( AttributeSet );
	check( AbilitySystemComponent );

	// For each tag bind callbacks to broadcast new value when attribute value changes
	for ( const auto& [Tag, Attribute] : AS->TagsToAttributes )
	{
		auto& OnAttributeChangeDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate( Attribute );
		OnAttributeChangeDelegate.AddLambda(
		    [this, Tag]( const FOnAttributeChangeData& ChangeData )
		    {
			    FAuraAttributeInfo AttributeInfo = AttributeInfoDataAsset->GetAttributeInfoByTag( Tag );
			    AttributeInfo.Value = ChangeData.NewValue;
			    OnAttributeInfoChanged.Broadcast( AttributeInfo );
		    } );
	}

	// PS Broadcasts to every client. But here we subscribe only to our own PSs OnPlayerStatChangedDelegate
	// Thats why on LevelUp or other broadcast only 1 OnPlayerStatChangedDynamicDelegate is broadcasted
	AAuraPlayerState* AuraPS = CastChecked<AAuraPlayerState>( PlayerState );
	AuraPS->OnAttributePointsChangedDelegate.AddLambda( [this]( int32 NewValue ) { OnAttributePointsChangedDynamicDelegate.Broadcast( NewValue ); } );
	AuraPS->OnSpellPointsChangedDelegate.AddLambda( [this]( int32 NewValue ) { OnSpellPointsChangedDynamicDelegate.Broadcast( NewValue ); } );
}
void UAuraAttributeWindowWC::BroadcastInitialValues() const
{
	// Go through all tags get their value and broadcast them
	const UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>( AttributeSet );
	for ( const auto& [Tag, Attribute] : AS->TagsToAttributes )
	{
		FAuraAttributeInfo AttributeInfo = AttributeInfoDataAsset->GetAttributeInfoByTag( Tag );
		AttributeInfo.Value = Attribute.GetNumericValue( AS );
		OnAttributeInfoChanged.Broadcast( AttributeInfo );
	}
}
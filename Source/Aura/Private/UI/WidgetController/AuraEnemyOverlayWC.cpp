// Dovzhik Tolya

#include "UI/WidgetController/AuraEnemyOverlayWC.h"

#include "GAS/AuraAbilitySystemComponent.h"
#include "GAS/AuraAttributeSet.h"

void UAuraEnemyOverlayWC::BroadcastInitialValues() const
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>( AttributeSet );
	OnHealthChanged.Broadcast( AuraAttributeSet->GetHealth() );
	OnMaxHealthChanged.Broadcast( AuraAttributeSet->GetMaxHealth() );
}
void UAuraEnemyOverlayWC::BindCallbacksToAttributeChanges() const
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>( AttributeSet );
	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>( AbilitySystemComponent );
	// AuraASC should always be a base class for ASC for this project
	check( AuraASC );

	// Bind callback to call it when Health attribute changes
	const FGameplayAttribute& HealthAttribute = AuraAttributeSet->GetHealthAttribute();
	auto& OnHealthChangeDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate( HealthAttribute );
	OnHealthChangeDelegate.AddLambda( [this]( const FOnAttributeChangeData& ChangeData ) { OnHealthChanged.Broadcast( ChangeData.NewValue ); } );

	// Bind callback to call it when MaxHealth attribute changes
	const FGameplayAttribute& MaxHealthAttribute = AuraAttributeSet->GetMaxHealthAttribute();
	auto& OnMaxHealthChangeDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate( MaxHealthAttribute );
	OnMaxHealthChangeDelegate.AddLambda( [this]( const FOnAttributeChangeData& ChangeData ) { OnMaxHealthChanged.Broadcast( ChangeData.NewValue ); } );
}
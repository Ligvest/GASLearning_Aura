// Dovzhik Tolya

#include "UI/WidgetController/AuraHUDWidgetController.h"

#include "GAS/AuraAttributeSet.h"

void UAuraHUDWidgetController::BroadcastInitialValues() const
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>( AttributeSet );
	OnHealthChanged.Broadcast( AuraAttributeSet->GetHealth() );
	OnMaxHealthChanged.Broadcast( AuraAttributeSet->GetMaxHealth() );
	OnManaChanged.Broadcast( AuraAttributeSet->GetMana() );
	OnMaxManaChanged.Broadcast( AuraAttributeSet->GetMaxMana() );
}
void UAuraHUDWidgetController::HealthChanged( const FOnAttributeChangeData& ChangeData ) const
{
	OnHealthChanged.Broadcast( ChangeData.NewValue );
}
void UAuraHUDWidgetController::MaxHealthChanged( const FOnAttributeChangeData& ChangeData ) const
{
	OnHealthChanged.Broadcast( ChangeData.NewValue );
}
void UAuraHUDWidgetController::ManaChanged( const FOnAttributeChangeData& ChangeData ) const
{
	OnManaChanged.Broadcast( ChangeData.NewValue );
}
void UAuraHUDWidgetController::MaxManaChanged( const FOnAttributeChangeData& ChangeData ) const
{
	OnMaxManaChanged.Broadcast( ChangeData.NewValue );
}
void UAuraHUDWidgetController::BindCallbacksToAttributeChanges() const
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>( AttributeSet );

	// Bind HealthChanged callback to call it when health attribute changes
	const FGameplayAttribute& HealthAttribute = AuraAttributeSet->GetHealthAttribute();
	auto& OnHealthChangeDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate( HealthAttribute );
	OnHealthChangeDelegate.AddUObject( this, &UAuraHUDWidgetController::HealthChanged );

	// Bind MaxHealthChanged callback to call it when health attribute changes
	const FGameplayAttribute& MaxHealthAttribute = AuraAttributeSet->GetMaxHealthAttribute();
	auto& OnMaxHealthChangeDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate( MaxHealthAttribute );
	OnMaxHealthChangeDelegate.AddUObject( this, &UAuraHUDWidgetController::MaxHealthChanged );

	// Bind ManaChanged callback to call it when health attribute changes
	const FGameplayAttribute& ManaAttribute = AuraAttributeSet->GetManaAttribute();
	auto& OnManaChangeDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate( ManaAttribute );
	OnManaChangeDelegate.AddUObject( this, &UAuraHUDWidgetController::ManaChanged );

	// Bind MaxManaChanged callback to call it when health attribute changes
	const FGameplayAttribute& MaxManaAttribute = AuraAttributeSet->GetMaxManaAttribute();
	auto& OnMaxManaChangeDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate( MaxManaAttribute );
	OnMaxManaChangeDelegate.AddUObject( this, &UAuraHUDWidgetController::MaxManaChanged );
}
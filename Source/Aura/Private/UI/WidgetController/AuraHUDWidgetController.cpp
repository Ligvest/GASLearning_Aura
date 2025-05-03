// Dovzhik Tolya

#include "UI/WidgetController/AuraHUDWidgetController.h"

#include "GAS/AuraAbilitySystemComponent.h"
#include "GAS/AuraAttributeSet.h"

void UAuraHUDWidgetController::BroadcastInitialValues() const
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>( AttributeSet );
	OnHealthChanged.Broadcast( AuraAttributeSet->GetHealth() );
	OnMaxHealthChanged.Broadcast( AuraAttributeSet->GetMaxHealth() );
	OnManaChanged.Broadcast( AuraAttributeSet->GetMana() );
	OnMaxManaChanged.Broadcast( AuraAttributeSet->GetMaxMana() );
}

void UAuraHUDWidgetController::BindCallbacksToAttributeChanges() const
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

	// Bind callback to call it when Mana attribute changes
	const FGameplayAttribute& ManaAttribute = AuraAttributeSet->GetManaAttribute();
	auto& OnManaChangeDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate( ManaAttribute );
	OnManaChangeDelegate.AddLambda( [this]( const FOnAttributeChangeData& ChangeData ) { OnManaChanged.Broadcast( ChangeData.NewValue ); } );

	// Bind callback to call it when MaxMana attribute changes
	const FGameplayAttribute& MaxManaAttribute = AuraAttributeSet->GetMaxManaAttribute();
	auto& OnMaxManaChangeDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate( MaxManaAttribute );
	OnMaxManaChangeDelegate.AddLambda( [this]( const FOnAttributeChangeData& ChangeData ) { OnMaxManaChanged.Broadcast( ChangeData.NewValue ); } );

	// Bind OnEffectWithTagsApplied to call when an effect with tags is applied
	AuraASC->OnEffectWithTagsAppliedDelegate.AddUObject( this, &UAuraHUDWidgetController::OnEffectWithTagsApplied );
}

void UAuraHUDWidgetController::OnEffectWithTagsApplied( const FGameplayTagContainer& TagContainer ) const
{
	FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag( FName( "Message" ) );
	for ( auto Tag : TagContainer )
	{
		if ( Tag.MatchesTag( MessageTag ) )
		{
			FEffectMessageRow* EffectMessageRow = GetDataTableRowByTag<FEffectMessageRow>( EffectMessageTable, Tag );
			EffectMessageRowDelegate.Broadcast( *EffectMessageRow );
		}
	}
}

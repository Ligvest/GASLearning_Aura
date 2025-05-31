// Dovzhik Tolya

#include "UI/WidgetController/AuraAttributeWindowWC.h"

#include "GAS/AuraAbilitySystemComponent.h"
#include "GAS/AuraAttributeSet.h"

void UAuraAttributeWindowWC::BindCallbacksToAttributeChanges() const
{
	/*
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>( AttributeSet );
	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>( AbilitySystemComponent );
	// AuraASC should always be a base class for ASC for this project
	check( AuraASC );

	// Bind callback to call it when MaxMana attribute changes
	const FGameplayAttribute& StrengthAttribute = AuraAttributeSet->GetStrengthAttribute();
	auto& OnStrengthChangeDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate( StrengthAttribute );
	OnStrengthChangeDelegate.AddLambda( [this]( const FOnAttributeChangeData& ChangeData ) { OnMaxManaChanged.Broadcast( ChangeData.NewValue ); } );

	// Bind OnEffectWithTagsApplied to call when an effect with tags is applied
	AuraASC->OnEffectWithTagsAppliedDelegate.AddUObject( this, &UAuraHUDWidgetController::OnEffectWithTagsApplied );
	*/
}
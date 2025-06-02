// Dovzhik Tolya

#include "GAS/AuraAbilitySystemComponent.h"

void UAuraAbilitySystemComponent::Init()
{
	InitSubscriptions();
}
void UAuraAbilitySystemComponent::InitSubscriptions()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject( this, &UAuraAbilitySystemComponent::OnEffectAppliedToSelf );
}
void UAuraAbilitySystemComponent::OnEffectAppliedToSelf( UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle EffectHandle )
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags( TagContainer );
	OnEffectWithTagsAppliedDelegate.Broadcast( TagContainer );
}
void UAuraAbilitySystemComponent::GrantAbilities( const TArray<TSubclassOf<UGameplayAbility>>& AbilityClasses )
{
	for ( auto AbilityClass : AbilityClasses )
	{
		constexpr int DefaultAbilityLevel = 1;
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec( AbilityClass, DefaultAbilityLevel );
		// GiveAbility(AbilitySpec);
		GiveAbilityAndActivateOnce( AbilitySpec );
	}
}
// Dovzhik Tolya

#include "GAS/AuraAbilitySystemComponent.h"

#include "GAS/Abilities/AuraGameplayAbility.h"

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
		if ( const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>( AbilitySpec.Ability ) )
		{
			// Add to dynamic tags a ability input tag to run it when the input fires up
			AbilitySpec.DynamicAbilityTags.AddTag( AuraAbility->DefaultAbilityTag );
			GiveAbility( AbilitySpec );
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagPressed( const FGameplayTag& InputTag )
{
	// TODO: Implement Pressed if needed
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld( const FGameplayTag& InputTag )
{
	if ( !InputTag.IsValid() ) return;

	// Get abilities which are not blocked by tags or other conditions and can be activated
	for ( FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities() )
	{
		// check if any of these abilities has the same tag as our input tag
		if ( AbilitySpec.DynamicAbilityTags.HasTagExact( InputTag ) )
		{
			// Activate the ability and set flag "Pressed"
			AbilitySpecInputPressed( AbilitySpec );
			if ( !AbilitySpec.IsActive() )
			{
				TryActivateAbility( AbilitySpec.Handle );
			}
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased( const FGameplayTag& InputTag )
{
	if ( !InputTag.IsValid() ) return;

	// Get abilities which are not blocked by tags or other conditions and can be activated
	for ( FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities() )
	{
		// check if any of these abilities has the same tag as our input tag
		if ( AbilitySpec.DynamicAbilityTags.HasTagExact( InputTag ) )
		{
			// Set flag "Released"
			AbilitySpecInputReleased( AbilitySpec );
		}
	}
}
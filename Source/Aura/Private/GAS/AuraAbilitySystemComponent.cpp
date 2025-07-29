// Dovzhik Tolya

#include "GAS/AuraAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "GAS/Abilities/AuraGameplayAbility.h"
#include "Interaction/PlayerInterface.h"

void UAuraAbilitySystemComponent::Init()
{
	InitSubscriptions();
}
void UAuraAbilitySystemComponent::InitSubscriptions()
{
	// This delegate is called only on a server.
	// So it doesn't replicate the call. Thats because we use an RPC instead of local function so that a server
	// replicated the call to the client for this ASC
	OnGameplayEffectAppliedDelegateToSelf.AddUObject( this, &UAuraAbilitySystemComponent::Client_OnEffectAppliedToSelf );
}
void UAuraAbilitySystemComponent::Client_OnEffectAppliedToSelf_Implementation( UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle EffectHandle )
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags( TagContainer );
	OnEffectWithTagsAppliedDelegate.Broadcast( TagContainer );
}
void UAuraAbilitySystemComponent::GrantAbilities( const TArray<TSubclassOf<UGameplayAbility>>& AbilityClasses, int AbilitiesLevel /* = 1 */ )
{
	for ( auto AbilityClass : AbilityClasses )
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec( AbilityClass, AbilitiesLevel );
		if ( const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>( AbilitySpec.Ability ) )
		{
			// Add to dynamic tags a ability input tag to run it when the input fires up
			// AbilitySpec.DynamicAbilityTags.AddTag( AuraAbility->DefaultAbilityTag ); // Deprecated
			AbilitySpec.GetDynamicSpecSourceTags().AddTag( AuraAbility->DefaultAbilityTag );
		}

		GiveAbility( AbilitySpec );
	}

	bStartupAbilitiesGranted = true;
	OnAbilitiesGrantedDelegate.Broadcast();
}

void UAuraAbilitySystemComponent::GrantPassiveAbilities( const TArray<TSubclassOf<UGameplayAbility>>& AbilityClasses, int AbilitiesLevel )
{
	for ( auto AbilityClass : AbilityClasses )
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec( AbilityClass, AbilitiesLevel );
		GiveAbilityAndActivateOnce( AbilitySpec );
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

void UAuraAbilitySystemComponent::ForEachAbility( const FForEachAbility& Delegate )
{
	FScopedAbilityListLock ActiveScopeLock( *this );
	for ( const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities() )
	{
		if ( !Delegate.ExecuteIfBound( AbilitySpec ) )
		{
			UE_LOG( LogTemp, Error, TEXT( "Failed to execute delegate in %hs" ), __FUNCTION__ );
		}
	}
}

void UAuraAbilitySystemComponent::UpgradeAttribute( const FGameplayTag AttributeTag )
{
	AActor* LocalAvatarActor = GetAvatarActor();
	if ( !LocalAvatarActor->Implements<UPlayerInterface>() )
	{
		return;
	}

	if ( IPlayerInterface::Execute_GetAttributePoints( LocalAvatarActor ) <= 0 )
	{
		return;
	}

	ServerUpgradeAttribute( AttributeTag );
}

void UAuraAbilitySystemComponent::ServerUpgradeAttribute_Implementation( const FGameplayTag& AttributeTag )
{
	AActor* LocalAvatarActor = GetAvatarActor();
	if ( !LocalAvatarActor->Implements<UPlayerInterface>() )
	{
		return;
	}

	// Decrement AttributePoints
	IPlayerInterface::Execute_AddToAttributePoints( LocalAvatarActor, -1 );

	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.f;

	// Send GameplayEvent to apply effect to increase attribute
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor( LocalAvatarActor, AttributeTag, Payload );
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityTagFromSpec( const FGameplayAbilitySpec& AbilitySpec )
{
	if ( AbilitySpec.Ability )
	{
		for ( FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags )
		{
			if ( Tag.MatchesTag( FGameplayTag::RequestGameplayTag( FName( "Abilities" ) ) ) )
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagFromSpec( const FGameplayAbilitySpec& AbilitySpec )
{
	for ( FGameplayTag Tag : AbilitySpec.DynamicAbilityTags )
	{
		// could it work?
		// FAuraGameplayTags::Get().InputPrefix;
		if ( Tag.MatchesTag( FGameplayTag::RequestGameplayTag( FName( "AuraInput" ) ) ) )
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	// We do these lines on server to send info about abilities to widgets
	// so the spell globes could set theirs ability icons
	// But to call these lines on cline we should use OnRep
	// Not sure if ActivateAbilities will be called after all abilities are set.
	if ( !bStartupAbilitiesGranted )
	{
		bStartupAbilitiesGranted = true;
		OnAbilitiesGrantedDelegate.Broadcast();
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
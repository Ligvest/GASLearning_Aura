// Dovzhik Tolya

#include "GAS/AuraAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "GAS/AuraGasBpLibrary.h"
#include "GAS/Abilities/AuraGameplayAbility.h"
#include "GAS/Data/AuraAbilityInfo_DA.h"
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

void UAuraAbilitySystemComponent::Client_UpdateAbilityStatus_Implementation( const FGameplayTag AbilityTag, const FGameplayTag StatusTag, int32 AbilityLevel )
{
	AbilityStatusChangedDelegate.Broadcast( AbilityTag, StatusTag, AbilityLevel );
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
			AbilitySpec.GetDynamicSpecSourceTags().AddTag( AuraAbility->DefaultAbilityInputTag );
			AbilitySpec.GetDynamicSpecSourceTags().AddTag( FAuraGameplayTags::Get().Abilities_Status_Equipped );
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
	if ( !InputTag.IsValid() ) return;

	// Get abilities which are not blocked by tags or other conditions and can be activated
	for ( FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities() )
	{
		// check if any of these abilities has the same tag as our input tag
		if ( AbilitySpec.DynamicAbilityTags.HasTagExact( InputTag ) )
		{
			// Activate the ability and set flag "Pressed"
			AbilitySpecInputPressed( AbilitySpec );
			if ( AbilitySpec.IsActive() )
			{
				// 	InvokeReplicatedEvent( EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, AbilitySpec.Ability->GetCurrentActivationInfo().GetActivationPredictionKey() );
				// }
				if ( UGameplayAbility* Instance = AbilitySpec.GetPrimaryInstance() )
				{
					const FPredictionKey& PredictionKey = Instance->GetCurrentActivationInfo().GetActivationPredictionKey();

					InvokeReplicatedEvent( EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, PredictionKey );
				}
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
			// InvokeReplicatedEvent( EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, AbilitySpec.Ability->GetCurrentActivationInfo().GetActivationPredictionKey() );
			if ( UGameplayAbility* Instance = AbilitySpec.GetPrimaryInstance() )
			{
				const FPredictionKey& PredictionKey = Instance->GetCurrentActivationInfo().GetActivationPredictionKey();

				InvokeReplicatedEvent( EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, PredictionKey );
			}
		}
	}
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

void UAuraAbilitySystemComponent::Server_SpendSpellPoint_Implementation( const FGameplayTag AbilityTag )
{
	if ( FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag( AbilityTag ) )
	{
		if ( GetAvatarActor()->Implements<UPlayerInterface>() )
		{
			IPlayerInterface::Execute_AddToSpellPoints( GetAvatarActor(), -1 );
		}

		const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
		FGameplayTag StatusTag = GetStatusTagFromSpec( *AbilitySpec );
		if ( StatusTag.MatchesTagExact( GameplayTags.Abilities_Status_Eligible ) )
		{
			AbilitySpec->DynamicAbilityTags.RemoveTag( GameplayTags.Abilities_Status_Eligible );
			AbilitySpec->DynamicAbilityTags.AddTag( GameplayTags.Abilities_Status_Unlocked );
			StatusTag = GameplayTags.Abilities_Status_Unlocked;
		}
		else if ( StatusTag.MatchesTagExact( GameplayTags.Abilities_Status_Equipped ) || StatusTag.MatchesTagExact( GameplayTags.Abilities_Status_Unlocked ) )
		{
			AbilitySpec->Level += 1;
		}

		Client_UpdateAbilityStatus( AbilityTag, StatusTag, AbilitySpec->Level );
		MarkAbilitySpecDirty( *AbilitySpec );
	}
}

void UAuraAbilitySystemComponent::Server_SetInputTagToSpec_Implementation( const FGameplayTag AbilityTag, const FGameplayTag Slot )
{
	if ( FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag( AbilityTag ) )
	{
		/*
		const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
		const FGameplayTag& Status = GetStatusTagFromSpec( *AbilitySpec );

		const bool bStatusValid = Status == GameplayTags.Abilities_Status_Equipped || Status == GameplayTags.Abilities_Status_Unlocked;
		if ( bStatusValid )
		{
		    // Remove this InputTag (slot) from any Ability that has it.
		    ClearAbilitiesFromInputTag( InputTag );
		    // Clear this ability's slot, just in case, it's a different slot
		    ClearInputTagFromAbility( AbilitySpec );
		    // Now, assign this ability to this slot
		    AbilitySpec->DynamicAbilityTags.AddTag( InputTag );
		    if ( Status.MatchesTagExact( GameplayTags.Abilities_Status_Unlocked ) )
		    {
		        AbilitySpec->DynamicAbilityTags.RemoveTag( GameplayTags.Abilities_Status_Unlocked );
		        AbilitySpec->DynamicAbilityTags.AddTag( GameplayTags.Abilities_Status_Equipped );
		    }

		    MarkAbilitySpecDirty( *AbilitySpec );
		}
		*/
		const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
		const FGameplayTag& PrevSlot = GetInputTagFromSpec( *AbilitySpec );
		const FGameplayTag& Status = GetStatusTagFromSpec( *AbilitySpec );

		const bool bStatusValid = Status == GameplayTags.Abilities_Status_Equipped || Status == GameplayTags.Abilities_Status_Unlocked;
		if ( bStatusValid )
		{
			// Handle activation/deactivation for passive abilities

			if ( !SlotIsEmpty( Slot ) )  // There is an ability in this slot already. Deactivate and clear its slot.
			{
				FGameplayAbilitySpec* SpecWithSlot = GetSpecWithSlot( Slot );
				if ( SpecWithSlot )
				{
					// is that ability the same as this ability? If so, we can return early.
					if ( AbilityTag.MatchesTagExact( GetAbilityTagFromSpec( *SpecWithSlot ) ) )
					{
						// Stephans broadcast Version:
						// ClientEquipAbility( AbilityTag, GameplayTags.Abilities_Status_Equipped, Slot, PrevSlot );
						Client_BroadcastAbilityEquipped();
						return;
					}

					if ( IsPassiveAbility( *SpecWithSlot ) )
					{
						FGameplayTag SlotAbilityTag = GetAbilityTagFromSpec( *SpecWithSlot );
						DeactivatePassiveAbility.Broadcast( SlotAbilityTag );
						Multicast_ActivatePassiveEffect( SlotAbilityTag, false );
					}

					ClearSlot( SpecWithSlot );
				}
			}

			if ( !AbilityHasAnySlot( *AbilitySpec ) )  // Ability doesn't yet have a slot (it's not active)
			{
				if ( IsPassiveAbility( *AbilitySpec ) )
				{
					TryActivateAbility( AbilitySpec->Handle );
					Multicast_ActivatePassiveEffect( AbilityTag, true );
				}
			}
			AssignSlotToAbility( *AbilitySpec, Slot );
			MarkAbilitySpecDirty( *AbilitySpec );
		}
		// Stephans broadcast Version:
		//  ClientEquipAbility(AbilityTag, GameplayTags.Abilities_Status_Equipped, Slot, PrevSlot);

		// It won't work on client. Because I modify ability here on Server and ask client to update its abilities.
		// But at the moment client still don't know anything about these changes as MarkAbilitySpecDirty only tells the server
		// to update the modified info on the next NetUpdate. So broadcasting only broadcasts old abilities info.
		// To fix this I should pass info to the broadcast and the process it to update it manually. But I don't want to spend time on it now
		Client_BroadcastAbilityEquipped();
	}
}

void UAuraAbilitySystemComponent::Client_BroadcastAbilityEquipped_Implementation()
{
	OnAbilityEquippedDelegate.Broadcast();
}

void UAuraAbilitySystemComponent::Multicast_ActivatePassiveEffect_Implementation( const FGameplayTag& AbilityTag, bool bActivate )
{
	ActivatePassiveEffect.Broadcast( AbilityTag, bActivate );
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityTagFromSpec( const FGameplayAbilitySpec& AbilitySpec )
{
	if ( AbilitySpec.Ability )
	{
		// could it work?
		// FAuraGameplayTags::Get().InputPrefix;
		for ( FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags )
		{
			if ( Tag.MatchesTag( FGameplayTag::RequestGameplayTag( FName( "Abilities" ) ) ) )
			{
				return Tag;
			}
		}
	}
	return FAuraGameplayTags::Get().None;
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
	return FAuraGameplayTags::Get().None;
}

void UAuraAbilitySystemComponent::SetInputTagToSpec( const FGameplayTag AbilityTag, const FGameplayTag InputTag )
{
	Server_SetInputTagToSpec( AbilityTag, InputTag );
}

FGameplayTag UAuraAbilitySystemComponent::GetStatusTagFromSpec( const FGameplayAbilitySpec& AbilitySpec )
{
	// could it work?
	// FAuraGameplayTags::Get().InputPrefix;

	for ( FGameplayTag StatusTag : AbilitySpec.DynamicAbilityTags )
	{
		if ( StatusTag.MatchesTag( FGameplayTag::RequestGameplayTag( FName( "Abilities.Status" ) ) ) )
		{
			return StatusTag;
		}
	}
	return FAuraGameplayTags::Get().None;
}

FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetSpecFromAbilityTag( const FGameplayTag& AbilityTag )
{
	FScopedAbilityListLock ActiveScopeLock( *this );
	for ( FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities() )
	{
		for ( FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags )
		{
			if ( Tag.MatchesTag( AbilityTag ) )
			{
				return &AbilitySpec;
			}
		}
	}
	return nullptr;
}

bool UAuraAbilitySystemComponent::SlotIsEmpty( const FGameplayTag& Slot )
{
	FScopedAbilityListLock ActiveScopeLoc( *this );
	for ( FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities() )
	{
		if ( AbilityHasSlot( AbilitySpec, Slot ) )
		{
			return false;
		}
	}
	return true;
}

bool UAuraAbilitySystemComponent::AbilityHasSlot( const FGameplayAbilitySpec& Spec, const FGameplayTag& Slot )
{
	return Spec.DynamicAbilityTags.HasTagExact( Slot );
}

bool UAuraAbilitySystemComponent::AbilityHasAnySlot( const FGameplayAbilitySpec& Spec )
{
	return Spec.DynamicAbilityTags.HasTag( FAuraGameplayTags::Get().AuraInput );
}

FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetSpecWithSlot( const FGameplayTag& Slot )
{
	FScopedAbilityListLock ActiveScopeLock( *this );
	for ( FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities() )
	{
		if ( AbilitySpec.DynamicAbilityTags.HasTagExact( Slot ) )
		{
			return &AbilitySpec;
		}
	}
	return nullptr;
}

bool UAuraAbilitySystemComponent::IsPassiveAbility( const FGameplayAbilitySpec& Spec ) const
{
	const UAuraAbilityInfo_DA* AbilityInfo = UAuraGasBpLibrary::GetAbilityInfoDA( GetAvatarActor() );
	const FGameplayTag AbilityTag = GetAbilityTagFromSpec( Spec );
	const FAuraAbilityInfo& Info = AbilityInfo->FindAbilityInfoForTag( AbilityTag );
	const FGameplayTag AbilityType = Info.AbilityTypeTag;
	return AbilityType.MatchesTagExact( FAuraGameplayTags::Get().Abilities_Type_Passive );
}

void UAuraAbilitySystemComponent::AssignSlotToAbility( FGameplayAbilitySpec& Spec, const FGameplayTag& Slot )
{
	ClearSlot( &Spec );
	Spec.DynamicAbilityTags.AddTag( Slot );
}

bool UAuraAbilitySystemComponent::GetDescriptionsByAbilityTag( const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription )
{
	if ( const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag( AbilityTag ) )
	{
		if ( UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>( AbilitySpec->Ability ) )
		{
			OutDescription = AuraAbility->GetDescription( AbilitySpec->Level );
			OutNextLevelDescription = AuraAbility->GetNextLevelDescription( AbilitySpec->Level + 1 );
			return true;
		}
	}

	if ( !AbilityTag.IsValid() || AbilityTag.MatchesTagExact( FAuraGameplayTags::Get().Abilities_None ) )
	{
		OutDescription = FString();
	}
	else
	{
		const UAuraAbilityInfo_DA* AbilityInfo = UAuraGasBpLibrary::GetAbilityInfoDA( GetAvatarActor() );
		OutDescription = UAuraGameplayAbility::GetLockedDescription( AbilityInfo->FindAbilityInfoForTag( AbilityTag ).LevelRequirement );
	}

	OutNextLevelDescription = FString();
	return false;
}

void UAuraAbilitySystemComponent::UpdateAbilityStatuses( int32 Level )
{
	UAuraAbilityInfo_DA* AbilityInfoDA = UAuraGasBpLibrary::GetAbilityInfoDA( GetAvatarActor() );
	for ( const FAuraAbilityInfo& Info : AbilityInfoDA->AbilityInformation )
	{
		if ( !Info.AbilityTag.IsValid() ) continue;
		if ( Level < Info.LevelRequirement ) continue;
		// This is very stupid function which iterate through all Grated abilities to get a tag.
		// So for real project develop something else to not get O(N^2)
		if ( GetSpecFromAbilityTag( Info.AbilityTag ) == nullptr )
		{
			constexpr int DefaultAbilityLevel = 1;
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec( Info.AbilityClass, DefaultAbilityLevel );
			const FGameplayTag NewStatusTag = FAuraGameplayTags::Get().Abilities_Status_Eligible;
			AbilitySpec.GetDynamicSpecSourceTags().AddTag( NewStatusTag );
			GiveAbility( AbilitySpec );
			// To force replication of the granted ability to client
			MarkAbilitySpecDirty( AbilitySpec );
			Client_UpdateAbilityStatus( Info.AbilityTag, NewStatusTag, DefaultAbilityLevel );
		}
	}
}

void UAuraAbilitySystemComponent::ClearSlot( FGameplayAbilitySpec* Spec )
{
	const FGameplayTag InputTag = GetInputTagFromSpec( *Spec );
	Spec->DynamicAbilityTags.RemoveTag( InputTag );
	MarkAbilitySpecDirty( *Spec );
}

void UAuraAbilitySystemComponent::ClearAbilitiesFromInputTag( const FGameplayTag& InputTag )
{
	FScopedAbilityListLock ActiveScopeLock( *this );
	for ( FGameplayAbilitySpec& Spec : GetActivatableAbilities() )
	{
		if ( AbilityHasSlot( Spec, InputTag ) )
		{
			ClearSlot( &Spec );
		}
	}
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
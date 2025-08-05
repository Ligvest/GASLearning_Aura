// Dovzhik Tolya

#include "GAS/AsyncTasks/WaitCooldownChange.h"
#include "AbilitySystemComponent.h"
#include "Algo/MaxElement.h"

UWaitCooldownChange* UWaitCooldownChange::WaitForCooldownChange( UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& InCooldownTag )
{
	UWaitCooldownChange* WaitCooldownChange = NewObject<UWaitCooldownChange>();
	WaitCooldownChange->ASC = AbilitySystemComponent;
	WaitCooldownChange->CooldownTag = InCooldownTag;

	if ( !IsValid( AbilitySystemComponent ) || !InCooldownTag.IsValid() )
	{
		WaitCooldownChange->EndTask();
		return nullptr;
	}

	// To know when a cooldown has ended (Cooldown Tag has been removed)
	AbilitySystemComponent->RegisterGameplayTagEvent( InCooldownTag, EGameplayTagEventType::NewOrRemoved ).AddUObject( WaitCooldownChange, &UWaitCooldownChange::CooldownTagChanged );

	// To know when a cooldown effect has been applied. And to get TimeRemaining from EffectSpec
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject( WaitCooldownChange, &UWaitCooldownChange::OnActiveEffectAdded );

	return WaitCooldownChange;
}

void UWaitCooldownChange::EndTask()
{
	if ( !IsValid( ASC ) ) return;
	ASC->RegisterGameplayTagEvent( CooldownTag, EGameplayTagEventType::NewOrRemoved ).RemoveAll( this );
	ASC->OnActiveGameplayEffectAddedDelegateToSelf.RemoveAll( this );

	// Allow GC to remove and free the object
	SetReadyToDestroy();
}

// Callback to call when a specific cooldown tag changed
void UWaitCooldownChange::CooldownTagChanged( const FGameplayTag InCooldownTag, int32 NewCount )
{
	if ( NewCount == 0 )
	{
		CooldownEnd.Broadcast( 0.f );
	}
}

// Callback to call when a effect is added. So we could get TimeRemaining for the cooldown tag
void UWaitCooldownChange::OnActiveEffectAdded( UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveEffectHandle )
{
	FGameplayTagContainer GrantedTags;
	SpecApplied.GetAllGrantedTags( GrantedTags );

	// This is not necessary to check existence of the tag before GetActiveEffectsTimeRemaining,
	// but it could be more performance efficient
	if ( GrantedTags.HasTagExact( CooldownTag ) )
	{
		// Query to get TimesRemaining for the single cooldown tag
		FGameplayEffectQuery GameplayEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags( CooldownTag.GetSingleTagContainer() );
		TArray<float> TimesRemaining = ASC->GetActiveEffectsTimeRemaining( GameplayEffectQuery );
		// If the result isn't empty
		if ( TimesRemaining.Num() > 0 )
		{
			// Get Maximum value if there are several.
			// There should be only one value but this is just a precation
			float MaxTimeRemaining = *Algo::MaxElement( TimesRemaining );

			bool bIsServer = TargetASC->GetOwnerRole() == ROLE_Authority;
			// Just for debugging purposes
			if ( bIsServer )
			{
				CooldownStart.Broadcast( MaxTimeRemaining );
			}
			else
			{
				// What chat GPT said:
				// On clients, this callback may fire twice:
				// First for the predicted effect (immediately on application),
				// Second when the server confirms and replicates the effect back.
				CooldownStart.Broadcast( MaxTimeRemaining );
			}
		}
	}
}

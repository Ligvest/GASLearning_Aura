// Dovzhik Tolya

#include "Player/AuraPlayerState.h"

#include "GAS/AuraAbilitySystemComponent.h"
#include "GAS/AuraAttributeSet.h"

void AAuraPlayerState::GetLifetimeReplicatedProps( TArray<class FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	// More detailed can check in AuraAttributeSet class
	DOREPLIFETIME_CONDITION_NOTIFY( AAuraPlayerState, XP, COND_None, REPNOTIFY_Always );
	DOREPLIFETIME_CONDITION_NOTIFY( AAuraPlayerState, Level, COND_None, REPNOTIFY_Always );
	DOREPLIFETIME_CONDITION_NOTIFY( AAuraPlayerState, AttributePoints, COND_None, REPNOTIFY_Always );
	DOREPLIFETIME_CONDITION_NOTIFY( AAuraPlayerState, SpellPoints, COND_None, REPNOTIFY_Always );
}

// Should be called only on Server.
// And clients will broadcast then new value in OnRep methods
void AAuraPlayerState::SetXP( int32 NewXP )
{
	if ( !HasAuthority() )
	{
		// Just try to make sure that we call this function solely on the server
		// Maybe we could just return if this is a client, but I want to pause the program here for learning purposes
		check( false );
	}

	XP = NewXP;
	OnXpChangedDelegate.Broadcast( XP );
	// TODO: Level = AuraLevelUpInfo_DA::FindLevel ?
}

// Should be called only on Server.
// And clients will broadcast then new value in OnRep methods
void AAuraPlayerState::AddToXP( int32 XpToAdd )
{
	if ( !HasAuthority() )
	{
		// Just try to make sure that we call this function solely on the server
		// Maybe we could just return if this is a client, but I want to pause the program here for learning purposes
		check( false );
	}
	XP += XpToAdd;
	OnXpChangedDelegate.Broadcast( XP );
	// TODO: Level = AuraLevelUpInfo_DA::FindLevel ?
}

void AAuraPlayerState::OnRep_XP( int32 OldXP )
{
	OnXpChangedDelegate.Broadcast( XP );
}

// Should be called only on Server.
// And clients will broadcast then new value in OnRep methods
void AAuraPlayerState::SetLevel( int32 NewLevel )
{
	if ( !HasAuthority() )
	{
		// Just try to make sure that we call this function solely on the server
		// Maybe we could just return if this is a client, but I want to pause the program here for learning purposes
		check( false );
	}
	Level = NewLevel;
	OnLevelChangedDelegate.Broadcast( Level, false );
}

// Should be called only on Server.
// And clients will broadcast then new value in OnRep methods
void AAuraPlayerState::AddToLevel( int32 LevelToAdd )
{
	if ( !HasAuthority() )
	{
		// Just try to make sure that we call this function solely on the server
		// Maybe we could just return if this is a client, but I want to pause the program here for learning purposes
		check( false );
	}
	Level += LevelToAdd;
	OnLevelChangedDelegate.Broadcast( Level, true );
}

void AAuraPlayerState::OnRep_Level( int32 OldLevel )
{
	// PS Broadcasts to every client. But in WC we subscribe only to our own PSs OnLevelChangedDelegate
	// Thats why on LevelUp only 1 WC::OnPlayerLevelChangedDynamicDelegate is broadcasted
	OnLevelChangedDelegate.Broadcast( Level, true );
}

// Attribute Points
// Should be called only on Server.
// And clients will broadcast then new value in OnRep methods
void AAuraPlayerState::SetAttributePoints( int32 NewAttributePoints )
{
	if ( !HasAuthority() )
	{
		// Just try to make sure that we call this function solely on the server
		// Maybe we could just return if this is a client, but I want to pause the program here for learning purposes
		check( false );
	}
	AttributePoints = NewAttributePoints;
	OnAttributePointsChangedDelegate.Broadcast( AttributePoints );
}
// Should be called only on Server.
// And clients will broadcast then new value in OnRep methods
void AAuraPlayerState::AddToAttributePoints( int32 AttributePointsToAdd )
{
	if ( !HasAuthority() )
	{
		// Just try to make sure that we call this function solely on the server
		// Maybe we could just return if this is a client, but I want to pause the program here for learning purposes
		check( false );
	}
	AttributePoints += AttributePointsToAdd;
	OnAttributePointsChangedDelegate.Broadcast( AttributePoints );
}
void AAuraPlayerState::OnRep_AttributePoints( int32 OldAttributePoints )
{
	OnAttributePointsChangedDelegate.Broadcast( AttributePoints );
}

// Should be called only on Server.
// And clients will broadcast then new value in OnRep methods
void AAuraPlayerState::SetSpellPoints( int32 NewSpellPoints )
{
	if ( !HasAuthority() )
	{
		// Just try to make sure that we call this function solely on the server
		// Maybe we could just return if this is a client, but I want to pause the program here for learning purposes
		check( false );
	}
	SpellPoints = NewSpellPoints;
	OnSpellPointsChangedDelegate.Broadcast( SpellPoints );
}
// Should be called only on Server.
// And clients will broadcast then new value in OnRep methods
void AAuraPlayerState::AddToSpellPoints( int32 SpellPointsToAdd )
{
	if ( !HasAuthority() )
	{
		// Just try to make sure that we call this function solely on the server
		// Maybe we could just return if this is a client, but I want to pause the program here for learning purposes
		check( false );
	}
	SpellPoints += SpellPointsToAdd;
	OnSpellPointsChangedDelegate.Broadcast( SpellPoints );
}
void AAuraPlayerState::OnRep_SpellPoints( int32 OldSpellPoints )
{
	OnSpellPointsChangedDelegate.Broadcast( SpellPoints );
}

AAuraPlayerState::AAuraPlayerState()
{
	// To replicate the PlayerState itself
	bReplicates = true;
	// Replicate 100 times a second I suppose
	SetNetUpdateFrequency( 100.f );

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>( "AuraAbilitySystemComponent" );

	// Enable replication of AbilitySystemComponent
	AbilitySystemComponent->SetIsReplicated( true );
	AbilitySystemComponent->SetReplicationMode( EGameplayEffectReplicationMode::Mixed );

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>( "AuraAttributeSet" );
}
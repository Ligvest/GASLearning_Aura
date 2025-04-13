// Dovzhik Tolya

#include "Player/AuraPlayerState.h"

#include "GAS/AuraAbilitySystemComponent.h"
#include "GAS/AuraAttributeSet.h"

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
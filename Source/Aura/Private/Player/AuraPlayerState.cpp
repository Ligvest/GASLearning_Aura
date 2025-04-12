// Dovzhik Tolya

#include "Player/AuraPlayerState.h"

#include "GAS/AuraAbilitySystemComponent.h"
#include "GAS/AuraAttributeSet.h"

AAuraPlayerState::AAuraPlayerState()
{
	// #lig Replication
	SetNetUpdateFrequency( 100.f );

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>( "AuraAbilitySystemComponent" );
	// #lig Replication
	AbilitySystemComponent->SetIsReplicated( true );
	AbilitySystemComponent->SetReplicationMode( EGameplayEffectReplicationMode::Mixed );

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>( "AuraAttributeSet" );
}
// Dovzhik Tolya

#include "GAS/AuraAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	// You can Init props in ctor but can't use Setter here
	InitHealth( 100.f );
	InitMaxHealth( 100.f );
	InitMana( 100.f );
	InitMaxMana( 100.f );
}

// If in UPROPERTY we specify HOW the field is replicated in the function we specify WHEN the prop is replicated
void UAuraAttributeSet::GetLifetimeReplicatedProps( TArray<class FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	// This call should works for UPROEPRTY(Replicated) only. To replicate always without any callbacks ( e.g. no OnRep_Health )
	// DOREPLIFETIME(UAuraAttributeSet, Health);
	/* COND_None - Replicate always unconditionally */
	/* REPNOTIFY_Always - each time call OnRep function ( OnRep_Health in this case ) */
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always );
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always );
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always );
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always );
}
void UAuraAttributeSet::OnRep_Health( const FGameplayAttributeData& OldHealth )
{
	// We can't just use Replicate as attributes replication in GAS should be forwarded to the GAS itself in the rep function
	// So we forward the replication notification further to GAS so it could proccess it correctly
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, Health, OldHealth );
}

void UAuraAttributeSet::OnRep_MaxHealth( const FGameplayAttributeData& OldMaxHealth )
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, MaxHealth, OldMaxHealth );
}
void UAuraAttributeSet::OnRep_Mana( const FGameplayAttributeData& OldMana )
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, Mana, OldMana );
}
void UAuraAttributeSet::OnRep_MaxMana( const FGameplayAttributeData& OldMaxMana )
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, MaxMana, OldMaxMana );
}
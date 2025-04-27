// Dovzhik Tolya

#include "GAS/AuraAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	// You can Init props in ctor but can't use Setter here
	InitHealth( 30.f );
	InitMaxHealth( 100.f );
	InitMana( 70.f );
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
void UAuraAttributeSet::OnRep_Health( const FGameplayAttributeData& OldHealth ) const
{
	// We can't just use Replicate as attributes replication in GAS should be forwarded to the GAS itself in the rep function
	// So we forward the replication notification further to GAS so it could proccess it correctly
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, Health, OldHealth );
}

void UAuraAttributeSet::OnRep_MaxHealth( const FGameplayAttributeData& OldMaxHealth ) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, MaxHealth, OldMaxHealth );
}
void UAuraAttributeSet::OnRep_Mana( const FGameplayAttributeData& OldMana ) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, Mana, OldMana );
}
void UAuraAttributeSet::OnRep_MaxMana( const FGameplayAttributeData& OldMaxMana ) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, MaxMana, OldMaxMana );
}
// This function is good for clamping CurrentValue of an attribute. To clamp BaseValue use PostGameplayEffectExecute
void UAuraAttributeSet::PreAttributeChange( const FGameplayAttribute& Attribute, float& NewValue )
{
	Super::PreAttributeChange( Attribute, NewValue );

	// Clamping CurrentValue of Health
	if ( Attribute == GetHealthAttribute() )
	{
		NewValue = FMath::Clamp( NewValue, 0.0f, GetMaxHealth() );
	}

	// Clamping CurrentValue of Mana
	if ( Attribute == GetManaAttribute() )
	{
		NewValue = FMath::Clamp( NewValue, 0.0f, GetMaxMana() );
	}
}
void UAuraAttributeSet::PostGameplayEffectExecute( const FGameplayEffectModCallbackData& Data )
{
	Super::PostGameplayEffectExecute( Data );

	// Clamping sketch
	// Clamping BaseValue of Health
	if ( Data.EvaluatedData.Attribute == GetHealthAttribute() )
	{
		SetHealth( FMath::Clamp( GetHealth(), 0.0f, GetMaxHealth() ) );
	}

	// Clamping BaseValue of Mana
	if ( Data.EvaluatedData.Attribute == GetManaAttribute() )
	{
		SetMana( FMath::Clamp( GetMana(), 0.0f, GetMaxMana() ) );
	}

	const FGameplayEffectContextHandle ContextHandle = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* SourceASCPtr = ContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	UAbilitySystemComponent& TargetASCRef = Data.Target;
	// Fill EffectTargetProps
	FillEffectPropertiesWithASC( EffectSourceProperties, SourceASCPtr, ContextHandle );
	// Fill EffectTargetProps
	FillEffectPropertiesWithASC( EffectTargetProperties, &TargetASCRef, ContextHandle );
}

void UAuraAttributeSet::FillEffectPropertiesWithASC( FEffectProperties& Properties, UAbilitySystemComponent* ASC, const FGameplayEffectContextHandle ContextHandle )
{
	// Set ASC. Ability System Component
	Properties.ASC = ASC;

	// Set Avatar. Representation in the World
	Properties.AvatarActor = ASC->GetAvatarActor();
	if ( !Properties.AvatarActor )
	{
		// If avatar is invalid then there are no Controller and Character
		return;
	}

	// Set Controller
	Properties.Controller = ASC->AbilityActorInfo->PlayerController.Get();  // Controller valid if controlled by player
	if ( !Properties.Controller )                                           // If not PlayerController then it could be AIController
	{
		if ( APawn* Pawn = Cast<APawn>( Properties.AvatarActor ) )
		{
			Properties.Controller = Pawn->GetController();
		}
	}

	// Set Character
	if ( Properties.Controller )
	{
		// If the actor is a character
		Properties.Character = Properties.Controller->GetCharacter();
	}
}

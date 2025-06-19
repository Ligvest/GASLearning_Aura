// Dovzhik Tolya

#include "GAS/AuraAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "GameplayEffectExtension.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/AuraPlayerController.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	/*
	// This is a bad way of initializing attributes.
	// Use datatables or GameplayEffects with "Override" option
	// Using GameplayEffects for initializing is preferable
	InitHealth( 30.f );
	InitMaxHealth( 100.f );
	InitMana( 70.f );
	InitMaxMana( 100.f );
	*/

	// Using GetXAttribute() functions should be safe. Just don't try to access numeric values before the Set is fully initialized
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	// Primary Attributes
	TagsToAttributes.Add( GameplayTags.Attributes_Primary_Strength, GetStrengthAttribute() );
	TagsToAttributes.Add( GameplayTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute() );
	TagsToAttributes.Add( GameplayTags.Attributes_Primary_Resilience, GetResilienceAttribute() );
	TagsToAttributes.Add( GameplayTags.Attributes_Primary_Vigor, GetVigorAttribute() );

	// Secondary Attributes
	TagsToAttributes.Add( GameplayTags.Attributes_Secondary_Armor, GetArmorAttribute() );
	TagsToAttributes.Add( GameplayTags.Attributes_Secondary_ArmorPenetration, GetArmorPenetrationAttribute() );
	TagsToAttributes.Add( GameplayTags.Attributes_Secondary_BlockChance, GetBlockChanceAttribute() );
	TagsToAttributes.Add( GameplayTags.Attributes_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute() );
	TagsToAttributes.Add( GameplayTags.Attributes_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute() );
	TagsToAttributes.Add( GameplayTags.Attributes_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute() );
	TagsToAttributes.Add( GameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute() );
	TagsToAttributes.Add( GameplayTags.Attributes_Secondary_HealthRegeneration, GetHealthRegenerationAttribute() );
	TagsToAttributes.Add( GameplayTags.Attributes_Secondary_MaxMana, GetMaxManaAttribute() );
	TagsToAttributes.Add( GameplayTags.Attributes_Secondary_ManaRegeneration, GetManaRegenerationAttribute() );
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
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always );

	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always );
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always );
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always );

	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always );
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always );
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, Resilience, COND_None, REPNOTIFY_Always );
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always );

	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, Armor, COND_None, REPNOTIFY_Always );
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always );
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, BlockChance, COND_None, REPNOTIFY_Always );
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always );
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always );
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always );
}

// Health
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
void UAuraAttributeSet::OnRep_HealthRegeneration( const FGameplayAttributeData& OldHealthRegeneration ) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, HealthRegeneration, OldHealthRegeneration );
}

// Mana
void UAuraAttributeSet::OnRep_Mana( const FGameplayAttributeData& OldMana ) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, Mana, OldMana );
}
void UAuraAttributeSet::OnRep_MaxMana( const FGameplayAttributeData& OldMaxMana ) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, MaxMana, OldMaxMana );
}
void UAuraAttributeSet::OnRep_ManaRegeneration( const FGameplayAttributeData& OldManaRegeneration ) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, ManaRegeneration, OldManaRegeneration );
}

// Primary attributes
void UAuraAttributeSet::OnRep_Strength( const FGameplayAttributeData& OldStrength ) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, Strength, OldStrength );
}
void UAuraAttributeSet::OnRep_Intelligence( const FGameplayAttributeData& OldIntelligence ) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, Intelligence, OldIntelligence );
}
void UAuraAttributeSet::OnRep_Resilience( const FGameplayAttributeData& OldResilience ) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, Resilience, OldResilience );
}
inline void UAuraAttributeSet::OnRep_Vigor( const FGameplayAttributeData& OldVigor ) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, Vigor, OldVigor );
}

// Secondary attributes
void UAuraAttributeSet::OnRep_Armor( const FGameplayAttributeData& OldArmor ) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, Armor, OldArmor );
}
void UAuraAttributeSet::OnRep_ArmorPenetration( const FGameplayAttributeData& OldArmorPenetration ) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, ArmorPenetration, OldArmorPenetration );
}
void UAuraAttributeSet::OnRep_BlockChance( const FGameplayAttributeData& OldBlockChance ) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, BlockChance, OldBlockChance );
}
void UAuraAttributeSet::OnRep_CriticalHitChance( const FGameplayAttributeData& OldCriticalHitChance ) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, CriticalHitChance, OldCriticalHitChance );
}
void UAuraAttributeSet::OnRep_CriticalHitDamage( const FGameplayAttributeData& OldCriticalHitDamage ) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, CriticalHitDamage, OldCriticalHitDamage );
}
void UAuraAttributeSet::OnRep_CriticalHitResistance( const FGameplayAttributeData& OldCriticalHitResistance ) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, CriticalHitResistance, OldCriticalHitResistance );
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

	const FGameplayEffectContextHandle ContextHandle = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* SourceASCPtr = ContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	UAbilitySystemComponent& TargetASCRef = Data.Target;
	// Fill EffectSourceProps. Source = causer of the effect
	FillEffectPropertiesWithASC( EffectSourceProperties, SourceASCPtr, ContextHandle );
	// Fill EffectTargetProps. Target = target of the effect (owner of this AS)
	FillEffectPropertiesWithASC( EffectTargetProperties, &TargetASCRef, ContextHandle );

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

	if ( Data.EvaluatedData.Attribute == GetIncomingDamageAttribute() )
	{
		// Consume damage
		float ReceivedDamage = GetIncomingDamage();
		SetIncomingDamage( 0.f );

		float NewHealth = GetHealth() - ReceivedDamage;
		SetHealth( std::clamp( NewHealth, 0.0f, GetMaxHealth() ) );

		bool bFatal = NewHealth <= 0.f;
		if ( bFatal )
		{
			// You are dead
			ICombatInterface* CombatActor = Cast<ICombatInterface>( EffectTargetProperties.AvatarActor );
			if ( CombatActor )
			{
				CombatActor->Die();
			}
		}
		else
		{
			FGameplayTagContainer TagContainer;
			TagContainer.AddTag( FAuraGameplayTags::Get().Effects_HitReact );
			EffectTargetProperties.ASC->TryActivateAbilitiesByTag( TagContainer );
		}

		// AuraAttributeSet is just a UObject which doesn't know about world so using TargetActor for world context object
		APlayerController* PC = UGameplayStatics::GetPlayerController( EffectTargetProperties.AvatarActor, 0 );
		AAuraPlayerController* AuraPC = Cast<AAuraPlayerController>( PC );

		if ( AuraPC && ( EffectSourceProperties.Character != EffectTargetProperties.Character ) )
		{
			AuraPC->ShowDamageNumber( ReceivedDamage, EffectTargetProperties.Character );
		}
	}

	// TODO: Debug
	UE_LOG( LogTemp, Warning, TEXT( "Changed Health on %s, Health: %f" ), *EffectTargetProperties.AvatarActor->GetName(), GetHealth() );
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

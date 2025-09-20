// Dovzhik Tolya

#include "GAS/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "GameplayEffectExtension.h"
#include "GAS/AuraGasBpLibrary.h"
#include "GameFramework/Character.h"
#include "GameplayEffectComponents/AssetTagsGameplayEffectComponent.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
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

	// Resistance Attributes
	TagsToAttributes.Add( GameplayTags.Attributes_Resistance_Fire, GetFireResistanceAttribute() );
	TagsToAttributes.Add( GameplayTags.Attributes_Resistance_Arcane, GetArcaneResistanceAttribute() );
	TagsToAttributes.Add( GameplayTags.Attributes_Resistance_Lightning, GetLightningResistanceAttribute() );
	TagsToAttributes.Add( GameplayTags.Attributes_Resistance_Physical, GetPhysicalResistanceAttribute() );
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

	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, ArcaneResistance, COND_None, REPNOTIFY_Always );
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, FireResistance, COND_None, REPNOTIFY_Always );
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, LightningResistance, COND_None, REPNOTIFY_Always );
	DOREPLIFETIME_CONDITION_NOTIFY( UAuraAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always );
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
void UAuraAttributeSet::OnRep_ArcaneResistance( const FGameplayAttributeData& OldArcaneResistance ) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, ArcaneResistance, OldArcaneResistance );
}
void UAuraAttributeSet::OnRep_FireResistance( const FGameplayAttributeData& OldFireResistance ) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, FireResistance, OldFireResistance );
}
void UAuraAttributeSet::OnRep_LightningResistance( const FGameplayAttributeData& OldLightningResistance ) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, LightningResistance, OldLightningResistance );
}
void UAuraAttributeSet::OnRep_PhysicalResistance( const FGameplayAttributeData& OldPhysicalResistance ) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY( UAuraAttributeSet, PhysicalResistance, OldPhysicalResistance );
}

// This function is good for clamping CurrentValue of an attribute. To clamp BaseValue use PostGameplayEffectExecute
// Is executed on both Server and Client
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

// Is executed only on Server
void UAuraAttributeSet::PostGameplayEffectExecute( const FGameplayEffectModCallbackData& Data )
{
	Super::PostGameplayEffectExecute( Data );

	const FGameplayEffectContextHandle ContextHandle = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* SourceASCPtr = ContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	UAbilitySystemComponent& TargetASCRef = Data.Target;
	// Fill EffectSourceProps. Source = causer of the effect
	FillEffectPropertiesWithASC( EffectSourceProperties, SourceASCPtr );
	// Fill EffectTargetProps. Target = target of the effect (owner of this AS)
	FillEffectPropertiesWithASC( EffectTargetProperties, &TargetASCRef );

	// Return if the target is Dead
	if ( /*EffectSourceProperties.Character && */ EffectTargetProperties.Character->Implements<UCombatInterface>() && ICombatInterface::Execute_IsDead( EffectTargetProperties.Character ) )
	{
		return;
	}

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

	// Calculate Debuff before ProcessIncomingDamage because in latter we process death.
	// And on death we remove any debuffs. So we don't want to remove debuffs on death and then add new ones after
	// Debuff
	if ( UAuraGasBpLibrary::GetIsDebuffSucceededFromEffectContext( ContextHandle ) )
	{
		ProcessDebuff( ContextHandle );
	}

	// IncomingDamage Meta Attribute
	if ( Data.EvaluatedData.Attribute == GetIncomingDamageAttribute() )
	{
		ProcessIncomingDamage( ContextHandle );
	}

	// IncomingXP Meta Attribute
	if ( Data.EvaluatedData.Attribute == GetIncomingXPAttribute() )
	{
		ProcessIncomingXP( ContextHandle );
	}
}
void UAuraAttributeSet::PostAttributeChange( const FGameplayAttribute& Attribute, float OldValue, float NewValue )
{
	Super::PostAttributeChange( Attribute, OldValue, NewValue );

	// We set bTopOffHealth on level up
	if ( Attribute == GetMaxHealthAttribute() && bTopOffHealth )
	{
		SetHealth( GetMaxHealth() );
		bTopOffHealth = false;
	}

	// We set bTopOffMana on level up
	if ( Attribute == GetMaxManaAttribute() && bTopOffMana )
	{
		SetMana( GetMaxMana() );
		bTopOffMana = false;
	}
}

void UAuraAttributeSet::ProcessIncomingDamage( const FGameplayEffectContextHandle& ContextHandle )
{
	// Consume damage
	float ReceivedDamage = GetIncomingDamage();
	SetIncomingDamage( 0.f );

	float NewHealth = GetHealth() - ReceivedDamage;
	SetHealth( std::clamp( NewHealth, 0.0f, GetMaxHealth() ) );

	bool bFatal = NewHealth <= 0.f;
	if ( bFatal )
	{
		ProcessDeath( ContextHandle );
	}
	else
	{
		if ( EffectTargetProperties.AvatarActor->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsBeingShocked( EffectTargetProperties.AvatarActor ) )
		{
			FGameplayTagContainer TagContainer;
			TagContainer.AddTag( FAuraGameplayTags::Get().Effects_HitReact );
			EffectTargetProperties.ASC->TryActivateAbilitiesByTag( TagContainer );
		}

		const FVector& KnockbackForce = UAuraGasBpLibrary::GetKnockbackImpulseFromEffectContext( ContextHandle );
		if ( !KnockbackForce.IsNearlyZero( 1.f ) )
		{
			EffectTargetProperties.Character->LaunchCharacter( KnockbackForce, true, true );
		}
	}

	ShowFloatingDamage( ContextHandle, ReceivedDamage );
}

void UAuraAttributeSet::ProcessIncomingXP( const FGameplayEffectContextHandle& ContextHandle )
{
	// Source Character is the owner, since GA_ListenForEvents applies GE_EventBasedEffect, adding to IncomingXP
	ACharacter* TargetCharacter = EffectTargetProperties.Character;
	ICombatInterface* CombatInterface = Cast<ICombatInterface>( TargetCharacter );
	if ( TargetCharacter->Implements<UPlayerInterface>() )
	{
		float ReceivedXP = GetIncomingXP();
		SetIncomingXP( 0.f );
		// UE_LOG( LogTemp, Warning, TEXT( "IncomingXP: %f" ), ReceivedXP );

		const int32 CurrentLevel = CombatInterface->GetCharacterLevel();
		const int32 CurrentXP = IPlayerInterface::Execute_GetXP( TargetCharacter );

		const int32 NewLevel = IPlayerInterface::Execute_FindLevelForXP( TargetCharacter, CurrentXP + ReceivedXP );
		const int32 NumLevelUps = NewLevel - CurrentLevel;
		if ( NumLevelUps > 0 )
		{
			int32 AttributePointsReward = 0;
			int32 SpellPointsReward = 0;

			for ( int32 i = 0; i < NumLevelUps; ++i )
			{
				SpellPointsReward += IPlayerInterface::Execute_GetSpellPointsReward( TargetCharacter, CurrentLevel + i );
				AttributePointsReward += IPlayerInterface::Execute_GetAttributePointsReward( TargetCharacter, CurrentLevel + i );
			}

			IPlayerInterface::Execute_AddToPlayerLevel( TargetCharacter, NumLevelUps );
			IPlayerInterface::Execute_AddToAttributePoints( TargetCharacter, AttributePointsReward );
			IPlayerInterface::Execute_AddToSpellPoints( TargetCharacter, SpellPointsReward );

			bTopOffHealth = true;
			bTopOffMana = true;

			IPlayerInterface::Execute_LevelUp( TargetCharacter );
		}

		IPlayerInterface::Execute_AddToXP( TargetCharacter, ReceivedXP );
	}
}

void UAuraAttributeSet::ProcessDeath( const FGameplayEffectContextHandle& ContextHandle ) const
{
	// You are dead
	AActor* TargetActor = EffectTargetProperties.AvatarActor;
	ICombatInterface* CombatActor = Cast<ICombatInterface>( TargetActor );
	if ( CombatActor )
	{
		FVector DeathImpulse = UAuraGasBpLibrary::GetDeathImpulseFromEffectContext( ContextHandle );
		CombatActor->Die( DeathImpulse );

		// Send Event With XP
		ECharacterClass CharacterClass = ICombatInterface::Execute_GetCharacterClass( TargetActor );
		int32 CharacterLevel = CombatActor->GetCharacterLevel();
		int32 XpReward = UAuraGasBpLibrary::GetXpRewardForClassAndLevel( TargetActor, CharacterClass, CharacterLevel );
		const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();
		FGameplayEventData Payload;
		// Seems like it's being overriden to the function argument whether I set it before or not
		// Payload.EventTag = Tags.Values_XP;
		Payload.EventMagnitude = XpReward;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor( EffectSourceProperties.Character, Tags.Attributes_Meta_IncomingXP, Payload );
	}
}

void UAuraAttributeSet::ProcessDebuff( const FGameplayEffectContextHandle& ContextHandle ) const
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	FGameplayEffectContextHandle NewContextHandle = EffectSourceProperties.ASC->MakeEffectContext();
	NewContextHandle.AddSourceObject( EffectSourceProperties.AvatarActor );

	// This is one way to pass data from ExecCalc to AttributeSet PostGameplayEffectExecute
	// But this is a stupid way as we still have access to GameplayEffectSpec from PostGameplayEffectExecute function
	// And from this Spec we easily can access all SetByCallerMagnitude data which already has all this shit
	// Also we can use SetByCallerMagnitude to pass a bool as a number.
	// So for learning purposes this is nice to try to make your custom EffectContext but in this situation you mustn't use it
	const FGameplayTag DamageTypeTag = UAuraGasBpLibrary::GetDamageTypeTagFromEffectContext( ContextHandle );
	const float DebuffDamage = UAuraGasBpLibrary::GetDebuffDamageFromEffectContext( ContextHandle );
	const float DebuffDuration = UAuraGasBpLibrary::GetDebuffDurationFromEffectContext( ContextHandle );
	const float DebuffFrequency = UAuraGasBpLibrary::GetDebuffFrequencyFromEffectContext( ContextHandle );
	check( DamageTypeTag.IsValid() );

	FString DebuffName = FString::Printf( TEXT( "DynamicDebuff_%s" ), *DamageTypeTag.ToString() );

	UGameplayEffect* Effect = NewObject<UGameplayEffect>( GetTransientPackage(), FName( DebuffName ) );
	Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	Effect->Period = DebuffFrequency;
	Effect->DurationMagnitude = FScalableFloat( DebuffDuration );
	Effect->StackingType = EGameplayEffectStackingType::AggregateBySource;
	Effect->StackLimitCount = 1;

	// Add tag to be able to distinguish debuff effects and remove them on character death
	UAssetTagsGameplayEffectComponent& AssetTagsComponent = Effect->FindOrAddComponent<UAssetTagsGameplayEffectComponent>();
	FInheritedTagContainer InheritedAssetTagContainer;
	InheritedAssetTagContainer.AddTag( GameplayTags.Debuff );
	AssetTagsComponent.SetAndApplyAssetTagChanges( InheritedAssetTagContainer );

	// Add tags to be granted on effect application
	UTargetTagsGameplayEffectComponent& TargetTagsComponent = Effect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	FInheritedTagContainer InheritedTagContainer;
	const FGameplayTag DebuffTag = GameplayTags.DamageTypeToDebuff[DamageTypeTag];
	InheritedTagContainer.AddTag( DebuffTag );
	if ( DebuffTag.MatchesTagExact( GameplayTags.Debuff_Stun ) )
	{
		InheritedTagContainer.AddTag( GameplayTags.Player_Block_CursorTrace );
		InheritedTagContainer.AddTag( GameplayTags.Player_Block_InputHeld );
		InheritedTagContainer.AddTag( GameplayTags.Player_Block_InputPressed );
		InheritedTagContainer.AddTag( GameplayTags.Player_Block_InputReleased );
	}

	TargetTagsComponent.SetAndApplyTargetTagChanges( InheritedTagContainer );

	// Add modifiers to modify IncomingDamage which will cause damage to target
	// More effective way as it lets to avoid redundant copying
	const int32 Index = Effect->Modifiers.Num();
	Effect->Modifiers.Add( FGameplayModifierInfo() );
	FGameplayModifierInfo& ModifierInfo = Effect->Modifiers[Index];
	ModifierInfo.ModifierMagnitude = FScalableFloat( DebuffDamage );
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.Attribute = GetIncomingDamageAttribute();

	// Make a GameplayEffectSpec
	FGameplayEffectSpec GameplayEffectSpec( Effect, NewContextHandle, 1.f );

	// Let IncomingDamage know of which damage type it is
	FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>( GameplayEffectSpec.GetContext().Get() );
	AuraContext->SetDamageTypeTag( DamageTypeTag );

	// Sooo. You can apply this effect, which is created dynamically right in the function and not as a separate asset.
	// But we have a problem here. If this asset is created dynamically here ( on server ) then client knows nothing about this GE.
	// So when we apply it from server to client, client won't know what tags to apply or anything else.
	// The correct way is to create an asset in Editor, and then you can fill it here and then send it to the client
	// If you will send Dynamically created GE then be ready to get many problems and to replicate all the tags and other things manually
	// In short: don't create GE dynamically if you want to replicate it and it's tags
	EffectTargetProperties.ASC->ApplyGameplayEffectSpecToSelf( GameplayEffectSpec );
}

void UAuraAttributeSet::ShowFloatingDamage( const FGameplayEffectContextHandle& ContextHandle, const int ReceivedDamage ) const
{
	// AuraAttributeSet is just a UObject which doesn't know about world so using TargetActor for world context object
	// We can't use UGameplayStatics::GetPlayerController as the PostGameplayEffectExecute is executed only on Server
	// so we would always get Server's local controller
	// ShowDamageNumber is replicated to all clients. All Controllers exist on Server. But only one exists on each Client
	// That's why the Damage Numbers appear only on a client with this controller
	AAuraPlayerController* AuraPC = EffectSourceProperties.Character->GetController<AAuraPlayerController>();
	// If Player is not Attacker
	if ( !AuraPC )
	{
		// To show numbers when player is being attacked by enemies
		AuraPC = EffectTargetProperties.Character->GetController<AAuraPlayerController>();
	}
	// If Player is not Being Attacked
	if ( !AuraPC )
	{
		return;
	}

	if ( EffectSourceProperties.Character != EffectTargetProperties.Character )
	{
		bool bIsBlockedHit = UAuraGasBpLibrary::GetIsBlockedHitFromEffectContext( ContextHandle );
		bool bIsCriticalHit = UAuraGasBpLibrary::GetIsCriticalHitFromEffectContext( ContextHandle );

		AuraPC->ShowDamageNumber( ReceivedDamage, EffectTargetProperties.Character, bIsBlockedHit, bIsCriticalHit );
	}

	/*
	// To show damage number when a player is being attacked
	AuraPC = EffectTargetProperties.Character->GetController<AAuraPlayerController>();
	if ( AuraPC && ( EffectSourceProperties.Character != EffectTargetProperties.Character ) )
	{
	    bool bIsBlockedHit = UAuraGasBpLibrary::IsBlockedHit( ContextHandle );
	    bool bIsCriticalHit = UAuraGasBpLibrary::IsCriticalHit( ContextHandle );
	    AuraPC->ShowDamageNumber( ReceivedDamage, EffectTargetProperties.Character, bIsBlockedHit, bIsCriticalHit );
	}
	*/
}

void UAuraAttributeSet::FillEffectPropertiesWithASC( FEffectProperties& Properties, UAbilitySystemComponent* ASC )
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

// Dovzhik Tolya

#include "GAS/AuraGasBpLibrary.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

// TODO: these functions return initialize and return WC. It's better to remove initialization from here in case
// we need to get the WC many time ( so that we don't initialize each time the parameters )
UAuraHUDWidgetController* UAuraGasBpLibrary::GetHudWC( const UObject* WorldContext )
{
	// Init Params
	FWidgetControllerParams WidgetControllerParams( GetWidgetControllerParams( WorldContext ) );

	// Get HUD
	AAuraHUD* HUD = WidgetControllerParams.PlayerController->GetHUD<AAuraHUD>();
	check( HUD );

	return HUD->TryGetHudWC( WidgetControllerParams );
}

UAuraAttributeWindowWC* UAuraGasBpLibrary::GetAttributeWindowWC( const UObject* WorldContext )
{
	// Init Params
	FWidgetControllerParams WidgetControllerParams( GetWidgetControllerParams( WorldContext ) );

	// Get HUD
	AAuraHUD* HUD = WidgetControllerParams.PlayerController->GetHUD<AAuraHUD>();
	check( HUD );

	return HUD->TryGetAttributeWindowWC( WidgetControllerParams );
}

UAuraSpellsWindowWC* UAuraGasBpLibrary::GetSpellsWindowWC( const UObject* WorldContext )
{
	// Init Params
	FWidgetControllerParams WidgetControllerParams( GetWidgetControllerParams( WorldContext ) );

	// Get HUD
	AAuraHUD* HUD = WidgetControllerParams.PlayerController->GetHUD<AAuraHUD>();
	check( HUD );

	return HUD->TryGetSpellsWindowWC( WidgetControllerParams );
}

// Server only
UAuraCharacterClassInfoDA* UAuraGasBpLibrary::GetCharacterClassInfoDA( const UObject* WorldContext )
{
	// GameMode available only on Server. Use GameState if you need to replicate GameMode specific data to clients
	AAuraGameModeBase* AuraGM = CastChecked<AAuraGameModeBase>( UGameplayStatics::GetGameMode( WorldContext ) );
	return AuraGM->GetDefaultCharacterInfoDA();
}

// Server only
UAuraAbilityInfo_DA* UAuraGasBpLibrary::GetAbilityInfoDA( const UObject* WorldContext )
{
	// GameMode available only on Server. Use GameState if you need to replicate GameMode specific data to clients
	AAuraGameModeBase* AuraGM = CastChecked<AAuraGameModeBase>( UGameplayStatics::GetGameMode( WorldContext ) );
	return AuraGM->GetAbilityInfoDA();
}

bool UAuraGasBpLibrary::GetIsBlockedHitFromEffectContext( const FGameplayEffectContextHandle& EffectContextHandle )
{
	if ( const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>( EffectContextHandle.Get() ) )
	{
		return AuraEffectContext->IsBlockedHit();
	}
	return false;
}

bool UAuraGasBpLibrary::GetIsCriticalHitFromEffectContext( const FGameplayEffectContextHandle& EffectContextHandle )
{
	if ( const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>( EffectContextHandle.Get() ) )
	{
		return AuraEffectContext->IsCriticalHit();
	}
	return false;
}

bool UAuraGasBpLibrary::GetIsDebuffSucceededFromEffectContext( const FGameplayEffectContextHandle& EffectContextHandle )
{
	if ( const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>( EffectContextHandle.Get() ) )
	{
		return AuraEffectContext->IsDebuffSucceeded();
	}
	return false;
}

float UAuraGasBpLibrary::GetDebuffDamageFromEffectContext( const FGameplayEffectContextHandle& EffectContextHandle )
{
	if ( const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>( EffectContextHandle.Get() ) )
	{
		return AuraEffectContext->GetDebuffDamage();
	}
	return false;
}

float UAuraGasBpLibrary::GetDebuffDurationFromEffectContext( const FGameplayEffectContextHandle& EffectContextHandle )
{
	if ( const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>( EffectContextHandle.Get() ) )
	{
		return AuraEffectContext->GetDebuffDuration();
	}
	return false;
}

float UAuraGasBpLibrary::GetDebuffFrequencyFromEffectContext( const FGameplayEffectContextHandle& EffectContextHandle )
{
	if ( const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>( EffectContextHandle.Get() ) )
	{
		return AuraEffectContext->GetDebuffFrequency();
	}
	return false;
}

FGameplayTag UAuraGasBpLibrary::GetDamageTypeTagFromEffectContext( const FGameplayEffectContextHandle& EffectContextHandle )
{
	if ( const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>( EffectContextHandle.Get() ) )
	{
		return AuraEffectContext->GetDamageTypeTag();
	}
	return FGameplayTag{};
}

FVector UAuraGasBpLibrary::GetDeathImpulseFromEffectContext( const FGameplayEffectContextHandle& EffectContextHandle )
{
	if ( const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>( EffectContextHandle.Get() ) )
	{
		return AuraEffectContext->GetDeathImpulse();
	}
	return FVector::ZeroVector;
}

FVector UAuraGasBpLibrary::GetKnockbackImpulseFromEffectContext( const FGameplayEffectContextHandle& EffectContextHandle )
{
	if ( const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>( EffectContextHandle.Get() ) )
	{
		return AuraEffectContext->GetKnockbackImpulse();
	}
	return FVector::ZeroVector;
}

void UAuraGasBpLibrary::SetIsBlockedHitFromEffectContext( FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit )
{
	if ( FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>( EffectContextHandle.Get() ) )
	{
		AuraEffectContext->SetIsBlockedHit( bInIsBlockedHit );
	}
}

void UAuraGasBpLibrary::SetIsCriticalHitInEffectContext( FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit )
{
	if ( FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>( EffectContextHandle.Get() ) )
	{
		AuraEffectContext->SetIsCriticalHit( bInIsCriticalHit );
	}
}

void UAuraGasBpLibrary::SetIsDebuffSucceededInEffectContext( FGameplayEffectContextHandle& EffectContextHandle, bool bInIsDebuffSucceeded )
{
	if ( FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>( EffectContextHandle.Get() ) )
	{
		AuraEffectContext->SetDebuffSucceeded( bInIsDebuffSucceeded );
	}
}

void UAuraGasBpLibrary::SetDebuffDamageInEffectContext( FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDamage )
{
	if ( FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>( EffectContextHandle.Get() ) )
	{
		AuraEffectContext->SetDebuffDamage( InDebuffDamage );
	}
}

void UAuraGasBpLibrary::SetDebuffDurationInEffectContext( FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDuration )
{
	if ( FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>( EffectContextHandle.Get() ) )
	{
		AuraEffectContext->SetDebuffDuration( InDebuffDuration );
	}
}

void UAuraGasBpLibrary::SetDebuffFrequencyInEffectContext( FGameplayEffectContextHandle& EffectContextHandle, float InDebuffFrequency )
{
	if ( FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>( EffectContextHandle.Get() ) )
	{
		AuraEffectContext->SetDebuffFrequency( InDebuffFrequency );
	}
}

void UAuraGasBpLibrary::SetDamageTypeTagInEffectContext( FGameplayEffectContextHandle& EffectContextHandle, FGameplayTag InDamageType )
{
	if ( FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>( EffectContextHandle.Get() ) )
	{
		AuraEffectContext->SetDamageTypeTag( InDamageType );
	}
}

void UAuraGasBpLibrary::SetDeathImpulseInEffectContext( FGameplayEffectContextHandle& EffectContextHandle, FVector InDeathImpulse )
{
	if ( FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>( EffectContextHandle.Get() ) )
	{
		AuraEffectContext->SetDeathImpulse( InDeathImpulse );
	}
}

void UAuraGasBpLibrary::SetKnockbackImpulseInEffectContext( FGameplayEffectContextHandle& EffectContextHandle, FVector InKnockbackImpulse )
{
	if ( FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>( EffectContextHandle.Get() ) )
	{
		AuraEffectContext->SetKnockbackImpulse( InKnockbackImpulse );
	}
}

void UAuraGasBpLibrary::GetLivePlayersInRadus( const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
                                               const FVector& SphereOrigin )
{
	// This code is just a low level versions of SphereOverlapActors and other methods of the same kind.
	// E.g UKismetSystemLibrary::SphereOverlapComponents
	// We use it to avoid unnecessary calculations
	const UWorld* World = GEngine->GetWorldFromContextObject( WorldContextObject, EGetWorldErrorMode::LogAndReturnNull );
	if ( World )
	{
		TArray<FOverlapResult> Overlaps;
		FCollisionQueryParams SphereParams;
		SphereParams.AddIgnoredActors( ActorsToIgnore );
		FCollisionObjectQueryParams CollisionObjectQueryParams = ( FCollisionObjectQueryParams::InitType::AllDynamicObjects );

		World->OverlapMultiByObjectType( Overlaps, SphereOrigin, FQuat::Identity, CollisionObjectQueryParams, FCollisionShape::MakeSphere( Radius ), SphereParams );
		for ( FOverlapResult& Overlap : Overlaps )
		{
			AActor* OverlappedActor = Overlap.GetActor();
			// Looking only for players
			if ( !HasPlayerActorTag( OverlappedActor ) )
			{
				continue;
			}

			bool IsCombatInterface = Overlap.GetActor()->Implements<UCombatInterface>();
			if ( IsCombatInterface && !ICombatInterface::Execute_IsDead( OverlappedActor ) )
			{
				OutOverlappingActors.AddUnique( ICombatInterface::Execute_GetAvatar( OverlappedActor ) );
			}
		}
	}
}

void UAuraGasBpLibrary::GetLiveOpponentsInRadus( const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
                                                 const FVector& SphereOrigin, const AActor* MyActor )
{
	// This code is just a low level versions of SphereOverlapActors and other methods of the same kind.
	// E.g UKismetSystemLibrary::SphereOverlapComponents
	// We use it to avoid unnecessary calculations
	const UWorld* World = GEngine->GetWorldFromContextObject( WorldContextObject, EGetWorldErrorMode::LogAndReturnNull );
	if ( World )
	{
		const FName OpponentTag = GetOpponentActorTag( MyActor );
		TArray<FOverlapResult> Overlaps;
		FCollisionQueryParams SphereParams;
		SphereParams.AddIgnoredActors( ActorsToIgnore );
		FCollisionObjectQueryParams CollisionObjectQueryParams = ( FCollisionObjectQueryParams::InitType::AllDynamicObjects );

		World->OverlapMultiByObjectType( Overlaps, SphereOrigin, FQuat::Identity, CollisionObjectQueryParams, FCollisionShape::MakeSphere( Radius ), SphereParams );
		for ( FOverlapResult& Overlap : Overlaps )
		{
			AActor* OverlappedActor = Overlap.GetActor();
			// Looking only for opponents to MyTag
			if ( !OverlappedActor->ActorHasTag( OpponentTag ) )
			{
				continue;
			}

			bool IsCombatInterface = Overlap.GetActor()->Implements<UCombatInterface>();
			if ( IsCombatInterface && !ICombatInterface::Execute_IsDead( OverlappedActor ) )
			{
				OutOverlappingActors.AddUnique( ICombatInterface::Execute_GetAvatar( OverlappedActor ) );
			}
		}
	}
}

AActor* UAuraGasBpLibrary::GetClosestPlayerInRadus( const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
                                                    const FVector& SphereOrigin )
{
	check( false );
	return nullptr;
}

FName UAuraGasBpLibrary::GetOpponentActorTag( const AActor* Actor )
{
	if ( Actor->ActorHasTag( GetEnemyActorTag() ) )
	{
		return GetPlayerActorTag();
	}

	if ( Actor->ActorHasTag( GetPlayerActorTag() ) )
	{
		return GetEnemyActorTag();
	}

	return FName();
}

int32 UAuraGasBpLibrary::GetXpRewardForClassAndLevel( const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 Level )
{
	UAuraCharacterClassInfoDA* CharacterClassInfoDA = UAuraGasBpLibrary::GetCharacterClassInfoDA( WorldContextObject );
	if ( !CharacterClassInfoDA )
	{
		return 0;
	}

	const FCharacterClassDefaultInfo& ClassInfo = CharacterClassInfoDA->GetClassDefaultInfo( CharacterClass );

	const float XpReward = ClassInfo.XpReward.GetValueAtLevel( Level );

	return static_cast<int32>( XpReward );
}

FActiveGameplayEffectHandle UAuraGasBpLibrary::ApplyDamageEffect( const FDamageEffectParams& DamageEffectParams )
{
	UAbilitySystemComponent* SourceASC = DamageEffectParams.SourceASC;
	UAbilitySystemComponent* TargetASC = DamageEffectParams.TargetASC;
	AActor* SourceAvatar = SourceASC->GetAvatarActor();
	const TSubclassOf<UGameplayEffect> EffectClass = DamageEffectParams.DamageGameplayEffectClass;
	int32 AbilityLevel = DamageEffectParams.AbilityLevel;

	FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject( SourceAvatar );
	SetDeathImpulseInEffectContext( EffectContextHandle, DamageEffectParams.DeathImpulse );
	SetKnockbackImpulseInEffectContext( EffectContextHandle, DamageEffectParams.KnockbackImpulse );
	const FGameplayEffectSpecHandle EffectSpecHandle = SourceASC->MakeOutgoingSpec( EffectClass, AbilityLevel, EffectContextHandle );

	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	EffectSpecHandle.Data->SetSetByCallerMagnitude( GameplayTags.Debuff_Chance, DamageEffectParams.DebuffChance );
	EffectSpecHandle.Data->SetSetByCallerMagnitude( GameplayTags.Debuff_Damage, DamageEffectParams.DebuffDamage );
	EffectSpecHandle.Data->SetSetByCallerMagnitude( GameplayTags.Debuff_Duration, DamageEffectParams.DebuffDuration );
	EffectSpecHandle.Data->SetSetByCallerMagnitude( GameplayTags.Debuff_Frequency, DamageEffectParams.DebuffFrequency );
	EffectSpecHandle.Data->SetSetByCallerMagnitude( DamageEffectParams.DamageTypeTag, DamageEffectParams.BaseDamage );
	EffectSpecHandle.Data->SetSetByCallerMagnitude( GameplayTags.Knockback_Chance, DamageEffectParams.KnockbackChance );
	// EffectSpecHandle.Data->SetSetByCallerMagnitude( GameplayTags.Knockback_Impulse, DamageEffectParams.KnockbackImpulse );

	return SourceASC->ApplyGameplayEffectSpecToTarget( *EffectSpecHandle.Data, TargetASC );
}

// I believe this function should be called after PlayerState replicated and is up to date
FWidgetControllerParams UAuraGasBpLibrary::GetWidgetControllerParams( const UObject* WorldContext )
{
	// I believe this function should be called after PlayerState replicated and is up to date
	// Init Params
	FWidgetControllerParams WidgetControllerParams;
	// PlayerController
	WidgetControllerParams.PlayerController = UGameplayStatics::GetPlayerController( WorldContext, 0 );
	check( WidgetControllerParams.PlayerController );

	// PlayerState
	WidgetControllerParams.PlayerState = WidgetControllerParams.PlayerController->PlayerState;
	// This function eventually is called from OnRep_PlayerState. When OnRep_PlayerState is called PlayerState and PlayerController should
	// be up to date. But they can not have links between each other yet. So if they are not linked yet we go back to a Pawn which should
	// be the Pawn which called OnRep_PlayerState and get the controller from this Pawn
	if ( !WidgetControllerParams.PlayerState )
	{
		// Pawn could be still not linked with the PlayerState but this case is when there are network problems
		// This game is intent to be Singleplayer but just to be aware
		APawn* ControlledPawn = WidgetControllerParams.PlayerController->GetPawn();
		if ( IsValid( ControlledPawn ) )
		{
			WidgetControllerParams.PlayerState = ControlledPawn->GetPlayerState();
		}
	}
	check( WidgetControllerParams.PlayerState );

	// AbilitySystemComponent
	const AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>( WidgetControllerParams.PlayerState );
	WidgetControllerParams.AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	check( WidgetControllerParams.AbilitySystemComponent );

	// AttributeSet
	WidgetControllerParams.AttributeSet = AuraPlayerState->GetAttributeSet();

	return WidgetControllerParams;
}
// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayTagContainer.h"
#include "Data/AuraCharacterClassInfoDA.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraGasBpLibrary.generated.h"

struct FDamageEffectParams;
struct FGameplayEffectContextHandle;
class UAuraHUDWidgetController;
/**
 *
 */
UCLASS()
class AURA_API UAuraGasBpLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION( BlueprintPure )
	static UAuraHUDWidgetController* GetHudWC( const UObject* WorldContext );

	UFUNCTION( BlueprintPure )
	static UAuraAttributeWindowWC* GetAttributeWindowWC( const UObject* WorldContext );

	UFUNCTION( BlueprintPure )
	static UAuraCharacterClassInfoDA* GetCharacterClassInfoDA( const UObject* WorldContext );

	UFUNCTION( BlueprintPure )
	static UAuraAbilityInfo_DA* GetAbilityInfoDA( const UObject* WorldContext );

	UFUNCTION( BlueprintPure, meta = ( DefaultToSelf = "WorldContext" ) )
	static UAuraSpellsWindowWC* GetSpellsWindowWC( const UObject* WorldContext );

	// I don't think that these functions are necessary here
	// but I added them in case Stephan will use them later in blueprints
	// ~ Begin of EffectContext variables
	// Getters
	UFUNCTION( BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static bool GetIsBlockedHitFromEffectContext( const FGameplayEffectContextHandle& EffectContextHandle );

	UFUNCTION( BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static bool GetIsCriticalHitFromEffectContext( const FGameplayEffectContextHandle& EffectContextHandle );

	UFUNCTION( BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static bool GetIsDebuffSucceededFromEffectContext( const FGameplayEffectContextHandle& EffectContextHandle );

	UFUNCTION( BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static float GetDebuffDamageFromEffectContext( const FGameplayEffectContextHandle& EffectContextHandle );

	UFUNCTION( BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static float GetDebuffDurationFromEffectContext( const FGameplayEffectContextHandle& EffectContextHandle );

	UFUNCTION( BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static float GetDebuffFrequencyFromEffectContext( const FGameplayEffectContextHandle& EffectContextHandle );

	UFUNCTION( BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static FGameplayTag GetDamageTypeTagFromEffectContext( const FGameplayEffectContextHandle& EffectContextHandle );

	UFUNCTION( BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static FVector GetDeathImpulseFromEffectContext( const FGameplayEffectContextHandle& EffectContextHandle );

	UFUNCTION( BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static FVector GetKnockbackImpulseFromEffectContext( const FGameplayEffectContextHandle& EffectContextHandle );

	/*
	UFUNCTION( BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static FVector GetKnockbackAmplitudeFromEffectContext( const FGameplayEffectContextHandle& EffectContextHandle );
	*/

	// Setters
	UFUNCTION( BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static void SetIsBlockedHitFromEffectContext( UPARAM( ref ) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit );

	UFUNCTION( BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static void SetIsCriticalHitInEffectContext( UPARAM( ref ) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit );

	UFUNCTION( BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static void SetIsDebuffSucceededInEffectContext( UPARAM( ref ) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsDebuffSucceeded );

	UFUNCTION( BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static void SetDebuffDamageInEffectContext( UPARAM( ref ) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDamage );

	UFUNCTION( BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static void SetDebuffDurationInEffectContext( UPARAM( ref ) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDuration );

	UFUNCTION( BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static void SetDebuffFrequencyInEffectContext( UPARAM( ref ) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffFrequency );

	UFUNCTION( BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static void SetDamageTypeTagInEffectContext( UPARAM( ref ) FGameplayEffectContextHandle& EffectContextHandle, FGameplayTag InDamageType );

	UFUNCTION( BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static void SetDeathImpulseInEffectContext( UPARAM( ref ) FGameplayEffectContextHandle& EffectContextHandle, FVector InDeathImpulse );

	UFUNCTION( BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static void SetKnockbackImpulseInEffectContext( UPARAM( ref ) FGameplayEffectContextHandle& EffectContextHandle, FVector InKnockbackImpulse );

	// ~ End of EffectContext variables

	UFUNCTION( BlueprintPure, Category = "AuraAbilitySystemLibrary|Interaction" )
	static void GetLivePlayersInRadus( const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin );

	UFUNCTION( BlueprintPure, Category = "AuraAbilitySystemLibrary|Interaction" )
	static void GetLiveOpponentsInRadus( const AActor* MyActor, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin );

	UFUNCTION( BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayMechanics" )
	static void GetClosestTargets( int32 MaxTargets, const TArray<AActor*>& Actors, TArray<AActor*>& OutClosestTargets, const FVector& Origin );

	UFUNCTION( BlueprintPure, Category = "AuraAbilitySystemLibrary|Interaction" )
	static AActor* GetClosestPlayerInRadus( const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
	                                        const FVector& SphereOrigin );

	UFUNCTION( BlueprintPure, Category = "Tags" )
	static FName GetEnemyActorTag() { return TEXT( "Enemy" ); };

	UFUNCTION( BlueprintPure, Category = "Tags" )
	static FName GetPlayerActorTag() { return TEXT( "Player" ); };

	// Return a tag which is an opposite ( or enemy ) to a given tag
	UFUNCTION( BlueprintPure, Category = "Tags" )
	static FName GetOpponentActorTag( const AActor* Actor );

	UFUNCTION( BlueprintPure, Category = "Tags" )
	static bool HasEnemyActorTag( const AActor* Actor ) { return Actor->ActorHasTag( GetEnemyActorTag() ); };

	UFUNCTION( BlueprintPure, Category = "Tags" )
	static bool HasPlayerActorTag( const AActor* Actor ) { return Actor->ActorHasTag( GetPlayerActorTag() ); };

	UFUNCTION( BlueprintPure, Category = "Tags" )
	static int32 GetXpRewardForClassAndLevel( const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 Level );

	UFUNCTION( BlueprintCallable, Category = "AuraAbilitySystemLibrary|DamageEffect" )
	static FActiveGameplayEffectHandle ApplyDamageEffect( const FDamageEffectParams& DamageEffectParams );

	UFUNCTION( BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayMechanics" )
	static TArray<FRotator> EvenlySpacedRotators( const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators );

	UFUNCTION( BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayMechanics" )
	static TArray<FVector> EvenlyRotatedVectors( const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors );

private:
	static FWidgetControllerParams GetWidgetControllerParams( const UObject* WorldContext );
};

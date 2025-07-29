// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "Data/AuraCharacterClassInfoDA.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraGasBpLibrary.generated.h"

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

	UFUNCTION( BlueprintPure, meta = ( DefaultToSelf = "WorldContext" ) )
	static UAuraSpellsWindowWC* GetSpellsWindowWC( const UObject* WorldContext );

	// I don't think that these functions are necessary here
	// but I added them in case Stephan will use them later in blueprints
	UFUNCTION( BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static bool IsBlockedHit( const FGameplayEffectContextHandle& EffectContextHandle );

	UFUNCTION( BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static bool IsCriticalHit( const FGameplayEffectContextHandle& EffectContextHandle );

	UFUNCTION( BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static void SetIsBlockedHit( UPARAM( ref ) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit );

	UFUNCTION( BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects" )
	static void SetIsCriticalHit( UPARAM( ref ) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit );

	UFUNCTION( BlueprintPure, Category = "AuraAbilitySystemLibrary|Interaction" )
	static void GetLivePlayersInRadus( const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin );

	UFUNCTION( BlueprintPure, Category = "AuraAbilitySystemLibrary|Interaction" )
	static void GetLiveOpponentsInRadus( const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin,
	                                     const AActor* MyActor );

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

private:
	static FWidgetControllerParams GetWidgetControllerParams( const UObject* WorldContext );
};

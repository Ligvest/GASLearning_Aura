// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "Data/AuraCharacterClassInfoDA.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraGasBpLibrary.generated.h"

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

	UFUNCTION( BlueprintPure, Category = "Tags" )
	static FName GetEnemyActorTag() { return TEXT( "Enemy" ); };

	UFUNCTION( BlueprintPure, Category = "Tags" )
	static FName GetPlayerActorTag() { return TEXT( "Player" ); };

	UFUNCTION( BlueprintPure, Category = "Tags" )
	static bool HasEnemyActorTag( const AActor* Actor ) { return Actor->ActorHasTag( GetEnemyActorTag() ); };

	UFUNCTION( BlueprintPure, Category = "Tags" )
	static bool HasPlayerActorTag( const AActor* Actor ) { return Actor->ActorHasTag( GetPlayerActorTag() ); };

private:
	static FWidgetControllerParams GetWidgetControllerParams( const UObject* WorldContext );
};

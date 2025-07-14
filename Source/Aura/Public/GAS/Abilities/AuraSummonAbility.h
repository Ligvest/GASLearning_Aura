// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/AuraGameplayAbility.h"
#include "AuraSummonAbility.generated.h"

class AAuraEnemyCharacter;
/**
 *
 */
UCLASS()
class AURA_API UAuraSummonAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
public:
	UFUNCTION( BlueprintCallable )
	TArray<FVector> GetSpawnLocations();

	UFUNCTION( BlueprintCallable )
	TSubclassOf<APawn> GetRandMinionClass();

protected:
	UPROPERTY( EditDefaultsOnly, Category = "Summoning" )
	int32 NumMinions = 5;

	UPROPERTY( EditDefaultsOnly, Category = "Summoning" )
	TArray<TSubclassOf<APawn>> MinionClasses;

	UPROPERTY( EditDefaultsOnly, Category = "Summoning" )
	float MinSpawnDistance = 150.f;

	UPROPERTY( EditDefaultsOnly, Category = "Summoning" )
	float MaxSpawnDistance = 400.f;

	UPROPERTY( EditDefaultsOnly, Category = "Summoning" )
	float SpawnSpreadAngle = 90.f;
};

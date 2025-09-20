// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "GAS/Data/AuraCharacterClassInfoDA.h"
#include "AuraEnemySpawnPoint.generated.h"

enum class ECharacterClass : uint8;
class AAuraEnemyCharacter;

/**
 *
 */
UCLASS()
class AURA_API AAuraEnemySpawnPoint : public ATargetPoint
{
	GENERATED_BODY()
public:
	UFUNCTION( BlueprintCallable )
	void SpawnEnemy();

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Enemy Class" )
	TSubclassOf<AAuraEnemyCharacter> EnemyClass;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Enemy Class" )
	int32 EnemyLevel = 1;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Enemy Class" )
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
};

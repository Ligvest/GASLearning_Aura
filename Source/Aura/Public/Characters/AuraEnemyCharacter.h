// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "AuraAICharacter.h"
#include "AuraEnemyCharacter.generated.h"

/**
 *
 */
UCLASS()
class AURA_API AAuraEnemyCharacter : public AAuraAICharacter
{
	AAuraEnemyCharacter();
	virtual void BeginPlay() override;
	GENERATED_BODY()
};

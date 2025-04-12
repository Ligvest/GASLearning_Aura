// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "Characters/AuraCharacterBase.h"
#include "AuraPlayerCharacter.generated.h"

/**
 *
 */
UCLASS()
class AURA_API AAuraPlayerCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()

public:
	AAuraPlayerCharacter();
	virtual void BeginPlay() override;
	virtual void PossessedBy( AController* NewController ) override;
	virtual void OnRep_PlayerState() override;

private:
	void InitAbilityActorInfo();
};

// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameModeBase.generated.h"

class UAuraAbilityInfo_DA;
class UAuraCharacterClassInfoDA;
/**
 *
 */
UCLASS()
class AURA_API AAuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	UAuraCharacterClassInfoDA* GetDefaultCharacterInfoDA();
	UAuraAbilityInfo_DA* GetAbilityInfoDA();

protected:
	UPROPERTY( EditDefaultsOnly, Category = "CharactersDefault" )
	TObjectPtr<UAuraCharacterClassInfoDA> DefaultCharacterInfoDA;

	UPROPERTY( EditDefaultsOnly, Category = "Abilities" )
	TObjectPtr<UAuraAbilityInfo_DA> AbilityInfoDA;
};

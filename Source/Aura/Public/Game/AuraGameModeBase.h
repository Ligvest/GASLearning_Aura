// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameModeBase.generated.h"

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

protected:
	UPROPERTY( EditDefaultsOnly, Category = "CharactersDefault" )
	TObjectPtr<UAuraCharacterClassInfoDA> DefaultCharacterInfoDA;
};

// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameModeBase.generated.h"

class ULoadScreenSaveGame;
class USaveGame;
class UMVVM_LoadSlot;
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

	void SaveSlotData( UMVVM_LoadSlot* LoadSlot, int32 SlotIndex );
	ULoadScreenSaveGame* GetSaveSlotData( const FString& SlotName, int32 SlotIndex ) const;
	static void DeleteSlot( const FString& SlotName, int32 SlotIndex );
	void TravelToMap( UMVVM_LoadSlot* Slot );

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY( EditDefaultsOnly )
	TSubclassOf<USaveGame> LoadScreenSaveGameClass;

	UPROPERTY( EditDefaultsOnly )
	FString DefaultMapName;

	UPROPERTY( EditDefaultsOnly )
	TSoftObjectPtr<UWorld> DefaultMap;

	UPROPERTY( EditDefaultsOnly )
	TMap<FString, TSoftObjectPtr<UWorld>> Maps;

protected:
	UPROPERTY( EditDefaultsOnly, Category = "CharactersDefault" )
	TObjectPtr<UAuraCharacterClassInfoDA> DefaultCharacterInfoDA;

	UPROPERTY( EditDefaultsOnly, Category = "Abilities" )
	TObjectPtr<UAuraAbilityInfo_DA> AbilityInfoDA;
};

// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameModeBase.generated.h"

class ULootTiersDA;
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

	// Saving
	void SaveSlotData( UMVVM_LoadSlot* LoadSlot, int32 SlotIndex );
	ULoadScreenSaveGame* GetSaveSlotData( const FString& SlotName, int32 SlotIndex ) const;
	static void DeleteSlot( const FString& SlotName, int32 SlotIndex );
	ULoadScreenSaveGame* RetrieveInGameSaveData();
	void SaveInGameProgressData( ULoadScreenSaveGame* SaveObject );
	void SaveWorldState( UWorld* World, const FString& DestinationMapAssetName = FString() ) const;
	void LoadWorldState( UWorld* World ) const;

	void TravelToMap( const FString& MapName );
	void PlayerDied( ACharacter* DeadCharacter );

protected:
	virtual AActor* ChoosePlayerStart_Implementation( AController* Player ) override;
	virtual void BeginPlay() override;

	// This is just a little workaround to get a Map name from a MapNameToMapPtr TMap
	// On production don't do like this and develop another way to save/load world.
	// Or you can use this save/load system but think how to use for example Enums or just Dungeon Assets to not convert name many times
	FString GetMapNameFromMapAssetName( const FString& MapAssetName ) const;

public:
	UPROPERTY( EditDefaultsOnly )
	TSubclassOf<USaveGame> LoadScreenSaveGameClass;

	UPROPERTY( EditDefaultsOnly )
	FName DefaultPlayerStartTag;

	UPROPERTY( EditDefaultsOnly )
	FString DefaultMapName;

	UPROPERTY( EditDefaultsOnly )
	TSoftObjectPtr<UWorld> DefaultMap;

	UPROPERTY( EditDefaultsOnly )
	TMap<FString, TSoftObjectPtr<UWorld>> MapNameToMapPtr;

	UPROPERTY( EditDefaultsOnly, Category = "Loot Tiers" )
	TObjectPtr<ULootTiersDA> LootTiers;

protected:
	UPROPERTY( EditDefaultsOnly, Category = "CharactersDefault" )
	TObjectPtr<UAuraCharacterClassInfoDA> DefaultCharacterInfoDA;

	UPROPERTY( EditDefaultsOnly, Category = "Abilities" )
	TObjectPtr<UAuraAbilityInfo_DA> AbilityInfoDA;
};

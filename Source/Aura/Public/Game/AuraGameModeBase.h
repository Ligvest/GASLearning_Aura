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
	UAuraCharacterClassInfoDA* GetDefaultCharacterInfoDA() const;
	UAuraAbilityInfo_DA* GetAbilityInfoDA() const;

	// Saving
	void SaveSlotData( UMVVM_LoadSlot* LoadSlot, int32 SlotIndex );
	ULoadScreenSaveGame* GetSaveSlotData( const FString& SlotName, int32 SlotIndex ) const;
	static void DeleteSlot( const FString& SlotName, int32 SlotIndex );
	ULoadScreenSaveGame* RetrieveInGameSaveData();
	void SaveInGameProgressData( ULoadScreenSaveGame* SaveObject );
	void SaveWorldState( UWorld* World, const FString& DestinationMapAssetName = FString() ) const;
	void LoadWorldState( UWorld* World ) const;
	// Loads the saved world state only once per map, no matter how many players get possessed
	void LoadWorldStateOnce( UWorld* World );

	// Only the host's own player reads and writes the save slot. Remote players start with defaults
	static bool IsSavingPlayer( const AController* Controller );
	// Calls SaveProgress for every player pawn. SaveProgress itself skips everyone but the host
	void SaveProgressForAllPlayers( const FName& PlayerStartTag ) const;

	// Uses ServerTravel so that connected clients travel together with the host.
	// GameMode exists only on the server, so callers get here only on the server.
	// Also use it in menu Blueprints instead of "Open Level": Get Game Mode -> Cast To AuraGameModeBase -> Travel To Map
	UFUNCTION( BlueprintCallable, Category = "Travel" )
	void TravelToMap( const TSoftObjectPtr<UWorld>& MapSoftPtr );
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

	// With several players a dead character is respawned instead of reloading the level. The dead body stays for this long
	UPROPERTY( EditDefaultsOnly, Category = "Multiplayer" )
	float DeadBodyLifeSpan = 5.f;

protected:
	// To avoid loading player attributes on subsequent calls of OnPossessed on player respawn.
	// Because in the OnPossessed we load the WorldState from saves
	bool bWorldStateLoaded = false;

	UPROPERTY( EditDefaultsOnly, Category = "CharactersDefault" )
	TObjectPtr<UAuraCharacterClassInfoDA> DefaultCharacterInfoDA;

	UPROPERTY( EditDefaultsOnly, Category = "Abilities" )
	TObjectPtr<UAuraAbilityInfo_DA> AbilityInfoDA;
};

// Dovzhik Tolya

#include "Game/AuraGameModeBase.h"

#include "EngineUtils.h"
#include "Game/AuraGameInstance.h"
#include "Game/LoadScreenSaveGame.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/SaveInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

UAuraCharacterClassInfoDA* AAuraGameModeBase::GetDefaultCharacterInfoDA()
{
	check( DefaultCharacterInfoDA );
	return DefaultCharacterInfoDA;
}
UAuraAbilityInfo_DA* AAuraGameModeBase::GetAbilityInfoDA()
{
	check( AbilityInfoDA );
	return AbilityInfoDA;
}

void AAuraGameModeBase::SaveSlotData( UMVVM_LoadSlot* LoadSlot, int32 SlotIndex )
{
	// If the SaveGame exists
	if ( UGameplayStatics::DoesSaveGameExist( LoadSlot->GetLoadSlotName(), SlotIndex ) )
	{
		// Delete the SaveGame
		UGameplayStatics::DeleteGameInSlot( LoadSlot->GetLoadSlotName(), SlotIndex );
	}

	// Create and fill a new SaveGame
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject( LoadScreenSaveGameClass );
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>( SaveGameObject );
	LoadScreenSaveGame->SaveSlotStatus = LoadSlot->SlotStatus;
	LoadScreenSaveGame->PlayerName = LoadSlot->GetPlayerName();
	LoadScreenSaveGame->DestinationMapName = LoadSlot->GetMapName();
	LoadScreenSaveGame->DestinationMapAssetName = LoadSlot->MapAssetName;
	LoadScreenSaveGame->PlayerStartTag = LoadSlot->PlayerStartTag;

	// Save the SaveGame object to disk
	UGameplayStatics::SaveGameToSlot( LoadScreenSaveGame, LoadSlot->GetLoadSlotName(), SlotIndex );
}

ULoadScreenSaveGame* AAuraGameModeBase::GetSaveSlotData( const FString& SlotName, int32 SlotIndex ) const
{
	USaveGame* SaveGameObject = nullptr;
	if ( UGameplayStatics::DoesSaveGameExist( SlotName, SlotIndex ) )
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot( SlotName, SlotIndex );
	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject( LoadScreenSaveGameClass );
	}
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>( SaveGameObject );
	return LoadScreenSaveGame;
}

void AAuraGameModeBase::DeleteSlot( const FString& SlotName, int32 SlotIndex )
{
	if ( UGameplayStatics::DoesSaveGameExist( SlotName, SlotIndex ) )
	{
		UGameplayStatics::DeleteGameInSlot( SlotName, SlotIndex );
	}
}

ULoadScreenSaveGame* AAuraGameModeBase::RetrieveInGameSaveData()
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>( GetGameInstance() );

	const FString InGameLoadSlotName = AuraGameInstance->LoadSlotName;
	const int32 InGameLoadSlotIndex = AuraGameInstance->LoadSlotIndex;

	return GetSaveSlotData( InGameLoadSlotName, InGameLoadSlotIndex );
}

void AAuraGameModeBase::SaveInGameProgressData( ULoadScreenSaveGame* SaveObject )
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>( GetGameInstance() );

	const FString InGameLoadSlotName = AuraGameInstance->LoadSlotName;
	const int32 InGameLoadSlotIndex = AuraGameInstance->LoadSlotIndex;
	AuraGameInstance->PlayerStartTag = SaveObject->PlayerStartTag;

	UGameplayStatics::SaveGameToSlot( SaveObject, InGameLoadSlotName, InGameLoadSlotIndex );
}

void AAuraGameModeBase::SaveWorldState( UWorld* World, const FString& DestinationMapAssetName ) const
{
	// Remove streaming prefix from WorldName
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart( World->StreamingLevelsPrefix );

	UAuraGameInstance* AuraGI = Cast<UAuraGameInstance>( GetGameInstance() );
	check( AuraGI );

	// Get SaveGame from disk or create a new one if it does not exist
	if ( ULoadScreenSaveGame* SaveGame = GetSaveSlotData( AuraGI->LoadSlotName, AuraGI->LoadSlotIndex ) )
	{
		// Add the World in SaveGame maps array if it isn't there
		// Not sure why we should store all maps in the SaveGame tbh. Why not just store one which we need to load?
		// This is very strange idea.
		// Okay. The idea is that we can travel between maps ( worlds ) and have saved states for each world.
		// So if I'll return to previous level it will have all world changes loaded
		if ( !SaveGame->HasMap( WorldName ) )
		{
			FSavedMap NewSavedMap;
			NewSavedMap.MapAssetName = WorldName;
			SaveGame->SavedMaps.Add( NewSavedMap );
		}

		if ( !DestinationMapAssetName.IsEmpty() )
		{
			SaveGame->DestinationMapAssetName = DestinationMapAssetName;
			SaveGame->DestinationMapName = GetMapNameFromMapAssetName( DestinationMapAssetName );
		}

		// Get a copy of this map from the SaveGame
		FSavedMap SavedMap = SaveGame->GetSavedMapWithMapName( WorldName );
		// Clear all Saved Actors from the map
		SavedMap.SavedActors.Empty();  // clear it out, we'll fill it in with "actors"

		// Iterator to iterate between all the actors in the World
		// In Editor can only iterate through current world actors ( so you can't iterate throug another world actors even if you specify it in constructor )
		for ( FActorIterator It( World ); It; ++It )
		{
			AActor* Actor = *It;

			bool bNonSavableActor = !IsValid( Actor ) || !Actor->Implements<USaveInterface>();
			if ( bNonSavableActor )
			{
				continue;
			}

			FSavedActor SavedActor;
			SavedActor.ActorName = Actor->GetFName();
			SavedActor.Transform = Actor->GetTransform();

			// MemoryWriter to serialize data to SavedActor.Bytes
			FMemoryWriter MemoryWriter( SavedActor.Bytes );
			// Archive to serialize FObjects and FNames to MemoryWriter
			FObjectAndNameAsStringProxyArchive Archive( MemoryWriter, true );
			// Mark that we serialize only fields with "SaveGame" properties
			Archive.ArIsSaveGame = true;
			// Serialize actor to Archive. Which will serialize to MemoryWriter. Which eventually will serialize to SavedActor.Bytes
			Actor->Serialize( Archive );
			// Add SavedActor to the array of actors in the SavedMap
			SavedMap.SavedActors.AddUnique( SavedActor );
		}

		// Find the Map in the array of SavedMaps of the SaveGame and override data with the new one
		// Why don't use the reference initially and do copy instead? Don't know. It would be better I believe
		for ( FSavedMap& MapToReplace : SaveGame->SavedMaps )
		{
			if ( MapToReplace.MapAssetName == WorldName )
			{
				MapToReplace = SavedMap;
				break;
			}
		}

		// Save all the data to disk
		UGameplayStatics::SaveGameToSlot( SaveGame, AuraGI->LoadSlotName, AuraGI->LoadSlotIndex );
	}
}

void AAuraGameModeBase::LoadWorldState( UWorld* World ) const
{
	// Remove streaming prefix from World name
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart( World->StreamingLevelsPrefix );

	UAuraGameInstance* AuraGI = Cast<UAuraGameInstance>( GetGameInstance() );
	check( AuraGI );

	if ( UGameplayStatics::DoesSaveGameExist( AuraGI->LoadSlotName, AuraGI->LoadSlotIndex ) )
	{
		// Get SaveGame from disk
		ULoadScreenSaveGame* SaveGame = Cast<ULoadScreenSaveGame>( UGameplayStatics::LoadGameFromSlot( AuraGI->LoadSlotName, AuraGI->LoadSlotIndex ) );
		if ( SaveGame == nullptr )
		{
			UE_LOG( LogTemp, Error, TEXT( "Failed to load slot" ) );
			return;
		}

		// SavedMap object which contains saved info about the World
		FSavedMap SavedMap = SaveGame->GetSavedMapWithMapName( WorldName );

		// Iterate through all actors in the World
		for ( FActorIterator It( World ); It; ++It )
		{
			AActor* Actor = *It;

			if ( !Actor->Implements<USaveInterface>() ) continue;

			// Iterate through all saved actors...
			for ( FSavedActor SavedActor : SavedMap.SavedActors )
			{
				// ...and find the one that corresponds to the one in the World
				if ( SavedActor.ActorName == Actor->GetFName() )
				{
					// Load transform if necessary
					if ( ISaveInterface::Execute_ShouldLoadTransform( Actor ) )
					{
						Actor->SetActorTransform( SavedActor.Transform );
					}
					// MemoryWriter to serialize data to SavedActor.Bytes
					FMemoryReader MemoryReader( SavedActor.Bytes );
					// Archive to deserialize FObjects and FNames from MemoryReader
					FObjectAndNameAsStringProxyArchive Archive( MemoryReader, true );
					// Mark that we serialize only fields with "SaveGame" properties
					Archive.ArIsSaveGame = true;
					// Deserialize actor from Archive. Which will deserialize from MemoryReader. Which eventually will be deserialized from SavedActor.Bytes
					Actor->Serialize( Archive );  // converts binary bytes back into variables

					// Execute Internal LoadActor if the actor has such functionality.
					// e.g. reached checkpoint should be visibly reached and have collision disabled
					ISaveInterface::Execute_LoadActor( Actor );
				}
			}
		}
	}
}

void AAuraGameModeBase::TravelToMap( const FString& MapName )
{
	TSoftObjectPtr<UWorld> MapSoftPtr = MapNameToMapPtr.FindChecked( MapName );
	UGameplayStatics::OpenLevelBySoftObjectPtr( this, MapSoftPtr );
}

AActor* AAuraGameModeBase::ChoosePlayerStart_Implementation( AController* Player )
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>( GetGameInstance() );

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass( GetWorld(), APlayerStart::StaticClass(), Actors );
	if ( Actors.Num() > 0 )
	{
		AActor* SelectedActor = Actors[0];
		for ( AActor* Actor : Actors )
		{
			if ( APlayerStart* PlayerStart = Cast<APlayerStart>( Actor ) )
			{
				if ( PlayerStart->PlayerStartTag == AuraGameInstance->PlayerStartTag )
				{
					SelectedActor = PlayerStart;
					break;
				}
			}
		}
		return SelectedActor;
	}
	return nullptr;
}

void AAuraGameModeBase::PlayerDied( ACharacter* DeadCharacter )
{
	ULoadScreenSaveGame* SaveGame = RetrieveInGameSaveData();
	if ( !IsValid( SaveGame ) ) return;

	UGameplayStatics::OpenLevel( DeadCharacter, FName( SaveGame->DestinationMapAssetName ) );
}

void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	MapNameToMapPtr.Add( DefaultMapName, DefaultMap );
}

// This is just a little workaround to get a Map name from a MapNameToMapPtr TMap
// On production don't do like this and develop another way to save/load world.
// Or you can use this save/load system but think how to use for example Enums or just Dungeon Assets to not convert name many times
FString AAuraGameModeBase::GetMapNameFromMapAssetName( const FString& MapAssetName ) const
{
	for ( const auto& [MapName, MapSoftPtr] : MapNameToMapPtr )
	{
		if ( MapSoftPtr.ToSoftObjectPath().GetAssetName() == MapAssetName )
		{
			return MapName;
		}
	}
	return FString();
}
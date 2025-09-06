// Dovzhik Tolya

#include "Game/AuraGameModeBase.h"

#include "Game/LoadScreenSaveGame.h"
#include "Kismet/GameplayStatics.h"
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
	LoadScreenSaveGame->MapName = LoadSlot->GetMapName();

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

void AAuraGameModeBase::TravelToMap( UMVVM_LoadSlot* Slot )
{
	const FString SlotName = Slot->GetLoadSlotName();
	const int32 SlotIndex = Slot->SlotIndex;

	UGameplayStatics::OpenLevelBySoftObjectPtr( Slot, Maps.FindChecked( Slot->GetMapName() ) );
}

void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	Maps.Add( DefaultMapName, DefaultMap );
}
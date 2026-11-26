// Dovzhik Tolya

#include "UI/ViewModel/MVVM_LoadScreen.h"

#include "Game/AuraGameInstance.h"
#include "Game/AuraGameModeBase.h"
#include "Game/LoadScreenSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadScreen::InitializeLoadSlots()
{
	// GameMode exists only on the server ( host or single player )
	bIsHost = IsValid( UGameplayStatics::GetGameMode( this ) );
	if ( !bIsHost && GEngine )
	{
		GEngine->AddOnScreenDebugMessage( -1, 30.f, FColor::Cyan, FString( "Connected to the host. Waiting for the host to start the game" ) );
	}

	LoadSlot_0 = NewObject<UMVVM_LoadSlot>( this, LoadSlotViewModelClass );
	LoadSlot_0->SetLoadSlotName( FString( "LoadSlot_0" ) );
	LoadSlot_0->SlotIndex = 0;
	LoadSlots.Add( 0, LoadSlot_0 );

	LoadSlot_1 = NewObject<UMVVM_LoadSlot>( this, LoadSlotViewModelClass );
	LoadSlot_1->SetLoadSlotName( FString( "LoadSlot_1" ) );
	LoadSlot_1->SlotIndex = 1;
	LoadSlots.Add( 1, LoadSlot_1 );

	LoadSlot_2 = NewObject<UMVVM_LoadSlot>( this, LoadSlotViewModelClass );
	LoadSlot_2->SetLoadSlotName( FString( "LoadSlot_2" ) );
	LoadSlot_2->SlotIndex = 2;
	LoadSlots.Add( 2, LoadSlot_2 );
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModelByIndex( int32 Index ) const
{
	return LoadSlots.FindChecked( Index );
}

void UMVVM_LoadScreen::NewSlotButtonPressed( int32 Slot, const FString& EnteredName )
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>( UGameplayStatics::GetGameMode( this ) );
	if ( !IsValid( AuraGameMode ) )
	{
		GEngine->AddOnScreenDebugMessage( 1, 15.f, FColor::Magenta, FString( "Only the host can create a save slot" ) );
		return;
	}

	LoadSlots[Slot]->SlotStatus = ESaveSlotStatus::Taken;
	LoadSlots[Slot]->SetPlayerName( EnteredName );
	LoadSlots[Slot]->SetPlayerLevel( 1 );
	LoadSlots[Slot]->SetMapName( AuraGameMode->DefaultMapName );
	LoadSlots[Slot]->PlayerStartTag = AuraGameMode->DefaultPlayerStartTag;
	LoadSlots[Slot]->MapAssetName = AuraGameMode->DefaultMap.ToSoftObjectPath().GetAssetName();
	LoadSlots[Slot]->InitializeSlot();

	AuraGameMode->SaveSlotData( LoadSlots[Slot], Slot );
}

void UMVVM_LoadScreen::NewGameButtonPressed( int32 Slot )
{
	LoadSlots[Slot]->SwitchWidgetDelegate.Broadcast( 1 );
}

void UMVVM_LoadScreen::SelectSlotButtonPressed( int32 InSlotIndex )
{
	OnSlotSelectedDelegate.Broadcast();
	for ( const auto& [LoadSlotIndex, LoadSlotViewModel] : LoadSlots )
	{
		if ( LoadSlotIndex == InSlotIndex )
		{
			LoadSlotViewModel->EnableSelectSlotButtonDelegate.Broadcast( false );
		}
		else
		{
			LoadSlotViewModel->EnableSelectSlotButtonDelegate.Broadcast( true );
		}
	}
	SelectedSlot = LoadSlots[InSlotIndex];
}

void UMVVM_LoadScreen::DeleteButtonPressed()
{
	// Save slots belong to the host
	if ( !bIsHost )
	{
		return;
	}

	if ( IsValid( SelectedSlot ) )
	{
		AAuraGameModeBase::DeleteSlot( SelectedSlot->GetLoadSlotName(), SelectedSlot->SlotIndex );
		SelectedSlot->SlotStatus = ESaveSlotStatus::Vacant;
		SelectedSlot->InitializeSlot();
		SelectedSlot->EnableSelectSlotButtonDelegate.Broadcast( true );
	}
}

void UMVVM_LoadScreen::PlayButtonPressed()
{
	if ( !IsValid( SelectedSlot ) )
	{
		return;
	}

	// Only the host starts the game. Clients are taken to the map by the host's ServerTravel
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>( UGameplayStatics::GetGameMode( this ) );
	if ( !IsValid( AuraGameMode ) )
	{
		return;
	}
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>( AuraGameMode->GetGameInstance() );
	AuraGameInstance->LoadSlotName = SelectedSlot->GetLoadSlotName();
	AuraGameInstance->LoadSlotIndex = SelectedSlot->SlotIndex;
	AuraGameInstance->PlayerStartTag = SelectedSlot->PlayerStartTag;

	AuraGameMode->TravelToMap( AuraGameMode->MapNameToMapPtr.FindChecked( SelectedSlot->GetMapName() ) );
}

void UMVVM_LoadScreen::LoadData()
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>( UGameplayStatics::GetGameMode( this ) );
	if ( !IsValid( AuraGameMode ) )
	{
		return;
	}

	for ( const auto& [LoadSlotIndex, LoadSlotViewModel] : LoadSlots )
	{
		ULoadScreenSaveGame* SaveObject = AuraGameMode->GetSaveSlotData( LoadSlotViewModel->GetLoadSlotName(), LoadSlotIndex );

		LoadSlotViewModel->SlotStatus = SaveObject->SaveSlotStatus;
		LoadSlotViewModel->SetPlayerName( SaveObject->PlayerName );
		LoadSlotViewModel->SetPlayerLevel( SaveObject->PlayerLevel );
		LoadSlotViewModel->SetMapName( SaveObject->DestinationMapName );
		LoadSlotViewModel->PlayerStartTag = SaveObject->PlayerStartTag;
		LoadSlotViewModel->InitializeSlot();
	}
}

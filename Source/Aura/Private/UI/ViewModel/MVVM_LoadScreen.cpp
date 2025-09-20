// Dovzhik Tolya

#include "UI/ViewModel/MVVM_LoadScreen.h"

#include "Game/AuraGameInstance.h"
#include "Game/AuraGameModeBase.h"
#include "Game/LoadScreenSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadScreen::InitializeLoadSlots()
{
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
		GEngine->AddOnScreenDebugMessage( 1, 15.f, FColor::Magenta, FString( "Please switch to Single Player" ) );
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

	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>( UGameplayStatics::GetGameMode( this ) );
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>( AuraGameMode->GetGameInstance() );
	AuraGameInstance->LoadSlotName = SelectedSlot->GetLoadSlotName();
	AuraGameInstance->LoadSlotIndex = SelectedSlot->SlotIndex;
	AuraGameInstance->PlayerStartTag = SelectedSlot->PlayerStartTag;

	AuraGameMode->TravelToMap( SelectedSlot->GetMapName() );
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

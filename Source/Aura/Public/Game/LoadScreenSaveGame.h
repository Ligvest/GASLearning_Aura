// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LoadScreenSaveGame.generated.h"

UENUM( BlueprintType )
enum class ESaveSlotStatus : uint8
{
	Vacant = 0,
	EnterName = 1,
	Taken = 2
};

/**
 *
 * A SaveGame object to store data specifically for LoadScreen widgets
 */
UCLASS()
class AURA_API ULoadScreenSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString SlotName = FString();

	UPROPERTY()
	int32 SlotIndex = 0;

	UPROPERTY()
	ESaveSlotStatus SaveSlotStatus = ESaveSlotStatus::Vacant;

	UPROPERTY()
	FString PlayerName = FString( "Default Name" );

	UPROPERTY()
	FString MapName = FString( "Default Map Name" );
};

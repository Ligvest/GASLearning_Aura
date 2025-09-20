// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadSlot.generated.h"

enum class ESaveSlotStatus : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FSwitchWidgetSignature, int32, WidgetSwitcherIndex );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FEnableSelectSlotButtonSignature, bool, bEnable );

/**
 *
 */
UCLASS()
class AURA_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	// Delegates
	UPROPERTY( BlueprintAssignable )
	FSwitchWidgetSignature SwitchWidgetDelegate;

	UPROPERTY( BlueprintAssignable )
	FEnableSelectSlotButtonSignature EnableSelectSlotButtonDelegate;

public:
	void InitializeSlot();

	// Setters
	void SetLoadSlotName( FString InLoadSlotName );
	void SetPlayerName( FString InPlayerName );
	void SetPlayerLevel( int32 InPlayerLevel );
	void SetMapName( FString InMapName );

	// Getters
	const FString& GetLoadSlotName() const { return LoadSlotName; }
	FString GetPlayerName() const { return PlayerName; }
	int32 GetPlayerLevel() const { return PlayerLevel; }
	FString GetMapName() const { return MapName; }

	UPROPERTY()
	int32 SlotIndex;

	UPROPERTY()
	ESaveSlotStatus SlotStatus;

	UPROPERTY()
	FName PlayerStartTag;

	UPROPERTY()
	FString MapAssetName;

private:
	UPROPERTY( BlueprintReadWrite, FieldNotify, Setter, Getter, meta = ( AllowPrivateAccess = "true" ) );
	FString LoadSlotName;

	UPROPERTY( BlueprintReadWrite, FieldNotify, Setter, Getter, meta = ( AllowPrivateAccess = "true" ) );
	FString PlayerName;

	UPROPERTY( BlueprintReadWrite, FieldNotify, Setter, Getter, meta = ( AllowPrivateAccess = "true" ) );
	int32 PlayerLevel;

	UPROPERTY( BlueprintReadWrite, FieldNotify, Setter, Getter, meta = ( AllowPrivateAccess = "true" ) );
	FString MapName;

	// A variable to bind widget to the ViewModel so the ViewModel would initialize properly.
	// I'll remove it when there will be a binding which makes sense instead of this workaround
	UPROPERTY( BlueprintReadWrite, FieldNotify, meta = ( AllowPrivateAccess = "true" ) )
	float Opacity = 1;
};

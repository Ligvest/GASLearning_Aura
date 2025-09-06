// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadScreen.generated.h"

class UMVVM_LoadSlot;

DECLARE_DYNAMIC_MULTICAST_DELEGATE( FSlotSelectedSignature );

/**
 *
 */
UCLASS()

// MVVM for Root LoadScreen Widget
class AURA_API UMVVM_LoadScreen : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	UPROPERTY( BlueprintAssignable )
	FSlotSelectedSignature OnSlotSelectedDelegate;

	void InitializeLoadSlots();

	UPROPERTY( EditDefaultsOnly )
	TSubclassOf<UMVVM_LoadSlot> LoadSlotViewModelClass;

	UFUNCTION( BlueprintPure )
	UMVVM_LoadSlot* GetLoadSlotViewModelByIndex( int32 Index ) const;

	// Button callbacks
	UFUNCTION( BlueprintCallable )
	void NewSlotButtonPressed( int32 Slot, const FString& EnteredName );

	UFUNCTION( BlueprintCallable )
	void NewGameButtonPressed( int32 Slot );

	UFUNCTION( BlueprintCallable )
	void SelectSlotButtonPressed( int32 InSlotIndex );

	UFUNCTION( BlueprintCallable )
	void DeleteButtonPressed();

	UFUNCTION( BlueprintCallable )
	void PlayButtonPressed();

	void LoadData();

private:
	// 3 slots. 1 slot for each WidgetSwitcher
	UPROPERTY()
	TMap<int32, UMVVM_LoadSlot*> LoadSlots;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_0;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_1;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_2;

	UPROPERTY()
	UMVVM_LoadSlot* SelectedSlot;

	// A variable to bind widget to the ViewModel so the ViewModel would initialize properly.
	// I'll remove it when there will be a binding which makes sense instead of this workaround
	UPROPERTY( BlueprintReadWrite, FieldNotify, meta = ( AllowPrivateAccess = "true" ) )
	float Opacity = 1;
};

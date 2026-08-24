// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayTags.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraSpellsWindowWC.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams( FSpellGlobeSelectedSignature, bool, bSpendPointsButtonEnabled, bool, bEquipButtonEnabled, FString, DescriptionString, FString,
                                               NextLevelDescriptionString );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FWaitForEquipSignature, const FGameplayTag, AbilityType );

struct FSelectedAbility
{
	FGameplayTag AbilityTag = FGameplayTag();
	FGameplayTag AbilityTypeTag = FGameplayTag();
	FGameplayTag StatusTag = FGameplayTag();
};
/**
 *
 */
UCLASS( Blueprintable )
class AURA_API UAuraSpellsWindowWC : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	UPROPERTY( BlueprintAssignable )
	FOnPlayerStatChangedDynamicSignature OnSpellPointsChangedDynamicDelegate;

public:
	UFUNCTION( BlueprintCallable )
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToAttributeChanges() override;

	UFUNCTION( BlueprintCallable )
	int32 GetSpellPoints();

	UPROPERTY( BlueprintAssignable )
	FSpellGlobeSelectedSignature SpellGlobeSelectedDelegate;

	UPROPERTY( BlueprintAssignable )
	FWaitForEquipSignature BeginWaitForEquipDelegate;

	UPROPERTY( BlueprintAssignable )
	FWaitForEquipSignature StopWaitForEquipDelegate;

	UPROPERTY( BlueprintAssignable )
	FSendSignalFromWCSignature ClearEquipGlobesDelegate;

	UPROPERTY( BlueprintAssignable )
	FSendSignalFromWCSignature AbilityEquippedDelegate;

	UFUNCTION( BlueprintCallable )
	void SpellGlobeSelected( const FGameplayTag AbilityTag );

	UFUNCTION( BlueprintCallable )
	void SpellGlobeDeselected();

	UFUNCTION( BlueprintCallable )
	void SpendPointButtonPressed();

	UFUNCTION( BlueprintCallable )
	void EquipButtonPressed();

	UFUNCTION( BlueprintCallable )
	void EquipGlobePressed( const FGameplayTag InputTag, const FGameplayTag AbilityTypeTag );

	void AbilityEquipped( FGameplayTag AbilityTag, FGameplayTag AbilitySkillMenuStatus, FGameplayTag NewSlot, FGameplayTag OldSlot );

	void BroadcastInfoOnSpellGlobeSelected( const int32 SpellPoints );

private:
	static void ShouldEnableButtons( const FGameplayTag& AbilityStatus, int32 SpellPoints, bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButton );
	FSelectedAbility SelectedAbility = { FAuraGameplayTags::Get().None, FAuraGameplayTags::Get().Abilities_Status_Locked };
	int32 CurrentSpellPoints = 0;
	bool bWaitingForEquipSelection = false;
};

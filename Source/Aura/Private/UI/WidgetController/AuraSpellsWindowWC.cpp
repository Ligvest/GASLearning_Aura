// Dovzhik Tolya

#include "UI/WidgetController/AuraSpellsWindowWC.h"

#include "AuraGameplayTags.h"
#include "GAS/AuraAbilitySystemComponent.h"
#include "GAS/Data/AuraAbilityInfo_DA.h"
#include "Player/AuraPlayerState.h"

void UAuraSpellsWindowWC::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	OnSpellPointsChangedDynamicDelegate.Broadcast( GetAuraPS()->GetSpellPoints() );
}

void UAuraSpellsWindowWC::BindCallbacksToAttributeChanges()
{
	GetAuraPS()->OnSpellPointsChangedDelegate.AddLambda(
	    [this]( int32 NewValue )
	    {
		    OnSpellPointsChangedDynamicDelegate.Broadcast( NewValue );
		    BroadcastInfoOnSpellGlobeSelected( NewValue );
	    } );

	GetAuraASC()->AbilityStatusChangedDelegate.AddLambda(
	    [this]( const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel )
	    {
		    if ( SelectedAbility.AbilityTag.MatchesTagExact( AbilityTag ) )
		    {
			    SelectedAbility.StatusTag = StatusTag;
			    BroadcastInfoOnSpellGlobeSelected( CurrentSpellPoints );
		    }

		    if ( AbilityInfoDataAsset )
		    {
			    FAuraAbilityInfo Info = AbilityInfoDataAsset->FindAbilityInfoForTag( AbilityTag );
			    Info.AbilityStatusTag = StatusTag;
			    AbilityInfoDelegate.Broadcast( Info );
		    }
	    } );

	GetAuraASC()->AbilityEquipped.AddUObject( this, &UAuraSpellsWindowWC::AbilityEquipped );
}

int32 UAuraSpellsWindowWC::GetSpellPoints()
{
	return GetAuraPS()->GetSpellPoints();
}

void UAuraSpellsWindowWC::SpellGlobeSelected( const FGameplayTag AbilityTag )
{
	if ( bWaitingForEquipSelection )
	{
		bWaitingForEquipSelection = false;
		StopWaitForEquipDelegate.Broadcast( SelectedAbility.AbilityTypeTag );
	}

	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	const int32 SpellPoints = GetAuraPS()->GetSpellPoints();

	FGameplayTag AbilityStatus;

	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag( GameplayTags.Abilities_None );
	const FGameplayAbilitySpec* AbilitySpec = GetAuraASC()->GetSpecFromAbilityTag( AbilityTag );
	const bool bSpecValid = AbilitySpec != nullptr;
	if ( !bTagValid || bTagNone || !bSpecValid )
	{
		AbilityStatus = GameplayTags.Abilities_Status_Locked;
		SelectedAbility.AbilityTypeTag = FAuraGameplayTags::Get().None;
	}
	else
	{
		AbilityStatus = GetAuraASC()->GetStatusTagFromSpec( *AbilitySpec );
		if ( AbilityInfoDataAsset )
		{
			FAuraAbilityInfo Info = AbilityInfoDataAsset->FindAbilityInfoForTag( AbilityTag );
			SelectedAbility.AbilityTypeTag = Info.AbilityTypeTag;
		}
	}

	SelectedAbility.AbilityTag = AbilityTag;
	SelectedAbility.StatusTag = AbilityStatus;
	BroadcastInfoOnSpellGlobeSelected( SpellPoints );
}

void UAuraSpellsWindowWC::SpellGlobeDeselected()
{
	if ( bWaitingForEquipSelection )
	{
		bWaitingForEquipSelection = false;
		StopWaitForEquipDelegate.Broadcast( SelectedAbility.AbilityTypeTag );
	}

	SelectedAbility.AbilityTag = FAuraGameplayTags::Get().Abilities_None;
	SelectedAbility.StatusTag = FAuraGameplayTags::Get().Abilities_Status_Locked;
	SelectedAbility.AbilityTypeTag = FAuraGameplayTags::Get().None;

	SpellGlobeSelectedDelegate.Broadcast( false, false, FString(), FString() );
}

void UAuraSpellsWindowWC::SpendPointButtonPressed()
{
	if ( GetAuraASC() )
	{
		GetAuraASC()->Server_SpendSpellPoint( SelectedAbility.AbilityTag );
	}
}

void UAuraSpellsWindowWC::EquipButtonPressed()
{
	BeginWaitForEquipDelegate.Broadcast( SelectedAbility.AbilityTypeTag );
	bWaitingForEquipSelection = true;
}

void UAuraSpellsWindowWC::EquipGlobePressed( const FGameplayTag InputTag, const FGameplayTag AbilityTypeTag )
{
	if ( !bWaitingForEquipSelection ) return;
	// Check selected ability against the slot's ability type.
	// (don't equip an offensive spell in a passive slot and vice versa)
	if ( !AbilityTypeTag.MatchesTagExact( SelectedAbility.AbilityTypeTag ) ) return;

	GetAuraASC()->SetInputTagToSpec( SelectedAbility.AbilityTag, InputTag );
}

void UAuraSpellsWindowWC::AbilityEquipped( FGameplayTag AbilityTag, FGameplayTag AbilitySkillMenuStatus, FGameplayTag NewSlot, FGameplayTag OldSlot )
{
	bWaitingForEquipSelection = false;

	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	FAuraAbilityInfo OldSlotInfo;
	OldSlotInfo.InputTag = OldSlot;
	OldSlotInfo.AbilityTag = GameplayTags.Abilities_None;
	AbilityInfoDelegate.Broadcast( OldSlotInfo );

	FAuraAbilityInfo NewSlotInfo = AbilityInfoDataAsset->FindAbilityInfoForTag( AbilityTag );
	NewSlotInfo.AbilityStatusTag = AbilitySkillMenuStatus;
	NewSlotInfo.InputTag = NewSlot;
	AbilityInfoDelegate.Broadcast( NewSlotInfo );

	StopWaitForEquipDelegate.Broadcast( NewSlotInfo.AbilityTypeTag );
	// 	AbilityEquippedDelegate.Broadcast();
}

void UAuraSpellsWindowWC::BroadcastInfoOnSpellGlobeSelected( const int32 SpellPoints )
{
	FString Description;
	FString NextLevelDescription;
	bool bEnableSpendPoints = false;
	bool bEnableEquip = false;
	ShouldEnableButtons( SelectedAbility.StatusTag, SpellPoints, bEnableSpendPoints, bEnableEquip );
	GetAuraASC()->GetDescriptionsByAbilityTag( SelectedAbility.AbilityTag, Description, NextLevelDescription );

	SpellGlobeSelectedDelegate.Broadcast( bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription );
}

void UAuraSpellsWindowWC::ShouldEnableButtons( const FGameplayTag& AbilityStatus, int32 SpellPoints, bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButton )
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	bShouldEnableSpellPointsButton = false;
	bShouldEnableEquipButton = false;
	if ( AbilityStatus.MatchesTagExact( GameplayTags.Abilities_Status_Equipped ) )
	{
		bShouldEnableEquipButton = true;
		if ( SpellPoints > 0 )
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
	else if ( AbilityStatus.MatchesTagExact( GameplayTags.Abilities_Status_Eligible ) )
	{
		if ( SpellPoints > 0 )
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
	else if ( AbilityStatus.MatchesTagExact( GameplayTags.Abilities_Status_Unlocked ) )
	{
		bShouldEnableEquipButton = true;
		if ( SpellPoints > 0 )
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
}
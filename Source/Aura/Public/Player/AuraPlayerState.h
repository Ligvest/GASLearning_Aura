// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "Net/UnrealNetwork.h"
#include "AuraPlayerState.generated.h"

// Forward declarations
class UAttributeSet;
class UAbilitySystemComponent;
class UAuraLevelUpInfo_DA;

DECLARE_MULTICAST_DELEGATE_OneParam( FOnPlayerStatChangedSignature, int32 /* Stat Value */ );

/**
 *
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAuraPlayerState();

	//~ Begin of IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; };
	//~ End of IAbilitySystemInterface

	// If in UPROPERTY we specify HOW the field is replicated in the function we specify WHEN the prop is replicated
	virtual void GetLifetimeReplicatedProps( TArray<class FLifetimeProperty>& OutLifetimeProps ) const override;

	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	// XP
	int32 GetXP() const { return XP; }
	void SetXP( int32 NewXP );
	void AddToXP( int32 XpToAdd );
	UFUNCTION()
	void OnRep_XP( int32 OldXP );

	// Player Level
	int32 GetPlayerLevel() const { return Level; }
	void SetLevel( int32 NewLevel );
	void AddToLevel( int32 LevelToAdd );
	UFUNCTION()
	void OnRep_Level( int32 OldLevel );

	// Attribute Points
	int32 GetPlayerAttributePoints() const { return AttributePoints; }
	void SetAttributePoints( int32 NewAttributePoints );
	void AddToAttributePoints( int32 AttributePointsToAdd );
	UFUNCTION()
	void OnRep_AttributePoints( int32 OldAttributePoints );

	// Spell Points
	int32 GetPlayerSpellPoints() const { return SpellPoints; }
	void SetSpellPoints( int32 NewSpellPoints );
	void AddToSpellPoints( int32 SpellPointsToAdd );
	UFUNCTION()
	void OnRep_SpellPoints( int32 OldSpellPoints );

	// Delegates:
	FOnPlayerStatChangedSignature OnXpChangedDelegate;
	FOnPlayerStatChangedSignature OnLevelChangedDelegate;
	FOnPlayerStatChangedSignature OnAttributePointsChangedDelegate;
	FOnPlayerStatChangedSignature OnSpellPointsChangedDelegate;

	UPROPERTY( EditDefaultsOnly )
	TObjectPtr<UAuraLevelUpInfo_DA> LevelUpInfo_DA;

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY( BlueprintReadOnly, ReplicatedUsing = OnRep_XP )
	int32 XP;

	UPROPERTY( BlueprintReadOnly, ReplicatedUsing = OnRep_Level )
	int32 Level = 1;

	UPROPERTY( BlueprintReadOnly, ReplicatedUsing = OnRep_AttributePoints )
	int32 AttributePoints = 1;

	UPROPERTY( BlueprintReadOnly, ReplicatedUsing = OnRep_SpellPoints )
	int32 SpellPoints = 1;
};

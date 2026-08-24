// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

class ULoadScreenSaveGame;
DECLARE_MULTICAST_DELEGATE_OneParam( FOnEffectWithTagsApplied, const FGameplayTagContainer& /* TagContainer */ );
DECLARE_MULTICAST_DELEGATE( FSendSignalFromASCSignature );
DECLARE_DELEGATE_OneParam( FForEachAbility, const FGameplayAbilitySpec& );
DECLARE_MULTICAST_DELEGATE_ThreeParams( FAbilityStatusChanged, const FGameplayTag /*AbilityTag*/, const FGameplayTag /*StatusTag*/, const int32 /* AbilityLevel */ );
DECLARE_MULTICAST_DELEGATE_FourParams( FAbilityEquipped, FGameplayTag /*AbilityTag*/, FGameplayTag /*AbilitySkillMenuStatus*/, FGameplayTag /*NewSlot*/, FGameplayTag /*OldSlot*/ );
DECLARE_MULTICAST_DELEGATE_OneParam( FDeactivatePassiveAbility, const FGameplayTag& /*AbilityTag*/ );
DECLARE_MULTICAST_DELEGATE_TwoParams( FActivatePassiveEffect, const FGameplayTag& /*AbilityTag*/, bool /*bActivate*/ );

/**
 *
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	void Init();

	bool bStartupAbilitiesGranted = false;

	FOnEffectWithTagsApplied OnEffectWithTagsAppliedDelegate;
	FSendSignalFromASCSignature OnAbilitiesGrantedDelegate;
	FSendSignalFromASCSignature OnAbilityEquippedDelegate;
	FAbilityStatusChanged AbilityStatusChangedDelegate;
	FAbilityEquipped AbilityEquipped;
	FDeactivatePassiveAbility DeactivatePassiveAbility;
	FActivatePassiveEffect ActivatePassiveEffect;

	void AddCharacterAbilitiesFromSaveData( ULoadScreenSaveGame* SaveData );
	void GrantAbilities( const TArray<TSubclassOf<UGameplayAbility>>& AbilityClasses, int AbilitiesLevel = 1 );
	void GrantPassiveAbilities( const TArray<TSubclassOf<UGameplayAbility>>& AbilityClasses, int AbilitiesLevel = 1 );
	void AbilityInputTagPressed( const FGameplayTag& InputTag );
	void AbilityInputTagReleased( const FGameplayTag& InputTag );
	void AbilityInputTagHeld( const FGameplayTag& InputTag );
	void ForEachAbility( const FForEachAbility& Delegate );

	void UpgradeAttribute( const FGameplayTag AttributeTag );

	UFUNCTION( Server, Reliable )
	void ServerUpgradeAttribute( const FGameplayTag& AttributeTag );

	UFUNCTION( Server, Reliable )
	void Server_SpendSpellPoint( const FGameplayTag AbilityTag );

	UFUNCTION( Server, Reliable )
	void Server_SetInputTagToSpec( const FGameplayTag AbilityTag, const FGameplayTag InputTag );

	// TODO: #lig Now Client_BroadcastAbilityEquipped and Client_EquipAbility duplicate each other. Must be merged for production
	UFUNCTION( Client, Reliable )
	void Client_BroadcastAbilityEquipped();

	UFUNCTION( Client, Reliable )
	void Client_EquipAbility( FGameplayTag AbilityTag, FGameplayTag AbilitySkillMenuStatus, FGameplayTag NewSlot, FGameplayTag OldSlot );

	UFUNCTION( NetMulticast, Unreliable )
	void Multicast_ActivatePassiveEffect( const FGameplayTag& AbilityTag, bool bActivate );

	void SetInputTagToSpec( FGameplayTag AbilityTag, const FGameplayTag InputTag );

	static FGameplayTag GetAbilityTagFromSpec( const FGameplayAbilitySpec& AbilitySpec );
	static FGameplayTag GetInputTagFromSpec( const FGameplayAbilitySpec& AbilitySpec );
	static FGameplayTag GetStatusTagFromSpec( const FGameplayAbilitySpec& AbilitySpec );
	FGameplayTag GetSlotFromAbilityTag( const FGameplayTag& AbilityTag );
	FGameplayTag GetStatusFromAbilityTag( const FGameplayTag& AbilityTag );
	FGameplayAbilitySpec* GetSpecFromAbilityTag( const FGameplayTag& AbilityTag );
	FGameplayAbilitySpec* GetSpecWithSlot( const FGameplayTag& Slot );

	bool SlotIsEmpty( const FGameplayTag& Slot );
	static bool AbilityHasSlot( const FGameplayAbilitySpec& Spec, const FGameplayTag& Slot );
	static bool AbilityHasAnySlot( const FGameplayAbilitySpec& Spec );
	bool IsPassiveAbility( const FGameplayAbilitySpec& Spec ) const;
	void AssignSlotToAbility( FGameplayAbilitySpec& Spec, const FGameplayTag& Slot );

	bool GetDescriptionsByAbilityTag( const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription );
	void UpdateAbilityStatuses( int32 Level );

protected:
	void ClearSlot( FGameplayAbilitySpec* Spec );
	void ClearAbilitiesFromInputTag( const FGameplayTag& InputTag );

	virtual void OnRep_ActivateAbilities() override;
	void InitSubscriptions();

	/** OnGameplayEffectAppliedDelegateToSelf Called on server whenever a GE is applied to self. This includes instant and duration based GEs. */
	UFUNCTION( Client, Reliable )
	void Client_OnEffectAppliedToSelf( UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle EffectHandle );

	UFUNCTION( Client, Reliable )
	void Client_UpdateAbilityStatus( const FGameplayTag AbilityTag, const FGameplayTag StatusTag, int32 AbilityLevel );
};

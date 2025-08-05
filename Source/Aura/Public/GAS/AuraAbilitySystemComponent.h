// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam( FOnEffectWithTagsApplied, const FGameplayTagContainer& /* TagContainer */ );
DECLARE_MULTICAST_DELEGATE( FSendSignalFromASCSignature );
DECLARE_DELEGATE_OneParam( FForEachAbility, const FGameplayAbilitySpec& );
DECLARE_MULTICAST_DELEGATE_ThreeParams( FAbilityStatusChanged, const FGameplayTag /*AbilityTag*/, const FGameplayTag /*StatusTag*/, const int32 /* AbilityLevel */ );

/**
 *
 */
UCLASS() class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	void Init();

	bool bStartupAbilitiesGranted = false;

	FOnEffectWithTagsApplied OnEffectWithTagsAppliedDelegate;
	FSendSignalFromASCSignature OnAbilitiesGrantedDelegate;
	FSendSignalFromASCSignature OnAbilityEquippedDelegate;
	FAbilityStatusChanged AbilityStatusChangedDelegate;

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

	UFUNCTION( Client, Reliable )
	void Client_BroadcastAbilityEquipped();

	static FGameplayTag GetAbilityTagFromSpec( const FGameplayAbilitySpec& AbilitySpec );
	static FGameplayTag GetInputTagFromSpec( const FGameplayAbilitySpec& AbilitySpec );
	void SetInputTagToSpec( FGameplayTag AbilityTag, const FGameplayTag InputTag );
	static FGameplayTag GetStatusTagFromSpec( const FGameplayAbilitySpec& AbilitySpec );
	FGameplayAbilitySpec* GetSpecFromAbilityTag( const FGameplayTag& AbilityTag );
	bool GetDescriptionsByAbilityTag( const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription );
	void UpdateAbilityStatuses( int32 Level );

protected:
	void ClearInputTagFromAbility( FGameplayAbilitySpec* Spec );
	void ClearAbilitiesFromInputTag( const FGameplayTag& InputTag );
	static bool AbilityHasInputTag( FGameplayAbilitySpec* Spec, const FGameplayTag& InputTag );

	virtual void OnRep_ActivateAbilities() override;
	void InitSubscriptions();

	/** OnGameplayEffectAppliedDelegateToSelf Called on server whenever a GE is applied to self. This includes instant and duration based GEs. */
	UFUNCTION( Client, Reliable )
	void Client_OnEffectAppliedToSelf( UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle EffectHandle );

	UFUNCTION( Client, Reliable )
	void Client_UpdateAbilityStatus( const FGameplayTag AbilityTag, const FGameplayTag StatusTag, int32 AbilityLevel );
};

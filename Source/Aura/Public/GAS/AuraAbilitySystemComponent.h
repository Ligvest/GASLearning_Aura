// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam( FOnEffectWithTagsApplied, const FGameplayTagContainer& /* TagContainer */ );
DECLARE_MULTICAST_DELEGATE( FOnAbilitiesGrantedDelegate );
DECLARE_DELEGATE_OneParam( FForEachAbility, const FGameplayAbilitySpec& );

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
	FOnAbilitiesGrantedDelegate OnAbilitiesGrantedDelegate;

	void GrantAbilities( const TArray<TSubclassOf<UGameplayAbility>>& AbilityClasses, int AbilitiesLevel = 1 );
	void GrantPassiveAbilities( const TArray<TSubclassOf<UGameplayAbility>>& AbilityClasses, int AbilitiesLevel = 1 );
	void AbilityInputTagPressed( const FGameplayTag& InputTag );
	void AbilityInputTagReleased( const FGameplayTag& InputTag );
	void AbilityInputTagHeld( const FGameplayTag& InputTag );
	void ForEachAbility( const FForEachAbility& Delegate );

	void UpgradeAttribute( const FGameplayTag AttributeTag );

	UFUNCTION( Server, Reliable )
	void ServerUpgradeAttribute( const FGameplayTag& AttributeTag );

	static FGameplayTag GetAbilityTagFromSpec( const FGameplayAbilitySpec& AbilitySpec );
	static FGameplayTag GetInputTagFromSpec( const FGameplayAbilitySpec& AbilitySpec );

protected:
	virtual void OnRep_ActivateAbilities() override;
	void InitSubscriptions();

	/** OnGameplayEffectAppliedDelegateToSelf Called on server whenever a GE is applied to self. This includes instant and duration based GEs. */
	UFUNCTION( Client, Reliable )
	void Client_OnEffectAppliedToSelf( UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle EffectHandle );
};

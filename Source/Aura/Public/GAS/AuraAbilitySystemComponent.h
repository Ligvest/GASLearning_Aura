// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam( FOnEffectWithTagsApplied, const FGameplayTagContainer& /* TagContainer */ );
/**
 *
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	void Init();

	FOnEffectWithTagsApplied OnEffectWithTagsAppliedDelegate;

private:
	void InitSubscriptions();

	/** OnGameplayEffectAppliedDelegateToSelf Called on server whenever a GE is applied to self. This includes instant and duration based GEs. */
	void OnEffectAppliedToSelf( UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle EffectHandle );
};

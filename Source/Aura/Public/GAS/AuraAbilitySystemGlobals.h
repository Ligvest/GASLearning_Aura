// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "AuraAbilitySystemGlobals.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UAuraAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

	// Override to use our custom GameplayEffectContext class
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};

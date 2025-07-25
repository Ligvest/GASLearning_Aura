// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GameplayTagContainer.h"
#include "ActiveGameplayEffectHandle.h"
#include "WaitCooldownChange.generated.h"

class UAbilitySystemComponent;
struct FGameplayEffectSpec;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FCooldownChangeSignature, float, TimeRemaining );

/**
 *
 */
//
// ExposedAsyncProxy = "AsyncTask" for all Async nodes return also their object pin
// In my case WaitForCooldownChange is an Async node
UCLASS( BlueprintType, meta = ( ExposedAsyncProxy = "AsyncTask" ) )
class AURA_API UWaitCooldownChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	// To broadcast when cooldown starts
	UPROPERTY( BlueprintAssignable )
	FCooldownChangeSignature CooldownStart;

	// To broadcast when cooldown ends
	UPROPERTY( BlueprintAssignable )
	FCooldownChangeSignature CooldownEnd;

	// The method which will create the node in BP
	UFUNCTION( BlueprintCallable, meta = ( BlueprintInternalUseOnly = "true" ) )
	static UWaitCooldownChange* WaitForCooldownChange( UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& InCooldownTag );

	// Call when the task should be finished
	UFUNCTION( BlueprintCallable )
	void EndTask();

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	FGameplayTag CooldownTag;

	// Callback to call when a specific cooldown tag changed
	void CooldownTagChanged( const FGameplayTag InCooldownTag, int32 NewCount );
	// Callback to call when a effect is added. So we could get TimeRemaining for the cooldown tag
	void OnActiveEffectAdded( UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveEffectHandle );
};

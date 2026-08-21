// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FMontageWaitSimpleDelegate, const FGameplayAbilityTargetDataHandle&, Data );

/**
 * This ability task replicates target data (a mouse hit result) using the GAS prediction system.
 * Although a simple RPC could be used to send FHitResult from client to server, we use the full GAS flow here
 * to take advantage of predictive execution, rollback support, and proper integration into the ability system pipeline.
 * This includes using PredictionKey to mark predictive actions, and the TargetData system for consistent handling of input data.
 * If the ability is cancelled, the prediction system can automatically revert client-side changes
 */
UCLASS()
class AURA_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()

public:
	// Implementing this static function is the AbilityTask implementation ( boilerplate )
	UFUNCTION( BlueprintCallable, Category = "Ability|Tasks",
	           meta = ( DisplayName = "TargetDataUnderMouse", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE" ) )
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse( UGameplayAbility* OwningAbility );

	// Adding MulticastDelegates add pins for your ability task node in blueprints
	// And these pins are executed on the delegate broadcast
	// Also parameters for these delegates also will become outputs in ability task node in blueprints
	UPROPERTY( BlueprintAssignable )
	FMontageWaitSimpleDelegate DataIsReady;

	// We implement this function to have a place to calculate needed data and broadcast our delegates and this calculated data
	virtual void Activate() override;

	// When data is received by server
	void OnTargetDataReplicatedCallback( const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag );

protected:
	void SendMouseCursorData();
};

// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE( MinimalAPI, BlueprintType )
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class AURA_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual int GetActorLevel() const = 0;
	virtual FVector GetProjectileSpawnSocketLocation() const = 0;

	UFUNCTION( BlueprintCallable, BlueprintImplementableEvent )
	void UpdateFacingLocation( FVector TargetLocation );

	// Why BlueprintNativeEvent?
	// We want to create a virtual function and implement in children
	// But interface doesn't let you have virtual C++ functions with BlueprintCallable specifier
	// So here is a workaround
	UFUNCTION( BlueprintCallable, BlueprintNativeEvent )
	UAnimMontage* GetHitReactMontage();
};

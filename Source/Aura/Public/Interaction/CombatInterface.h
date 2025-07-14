// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class AAuraCharacterBase;
class UNiagaraSystem;

USTRUCT( BlueprintType )
struct FTaggedMontage
{
	GENERATED_BODY()

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	TObjectPtr<UAnimMontage> Montage;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	FGameplayTag MontageTag;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	FGameplayTag CombatSocketTag;

	// Projectiles have it's own HitSound which is triggered on overlap
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	TObjectPtr<USoundBase> MeleeHitSound;
};

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

	UFUNCTION( BlueprintCallable, BlueprintNativeEvent )
	FVector GetCombatSocketLocation( FGameplayTag MontageAttackTag ) const;

	UFUNCTION( BlueprintCallable, BlueprintImplementableEvent )
	void UpdateFacingLocation( FVector TargetLocation );

	// Why BlueprintNativeEvent?
	// We want to create a virtual function and implement in children
	// But interface doesn't let you have virtual C++ functions with BlueprintCallable specifier
	// So here is a workaround
	UFUNCTION( BlueprintCallable, BlueprintNativeEvent )
	UAnimMontage* GetHitReactMontage();

	virtual void Die() = 0;

	UFUNCTION( BlueprintCallable, BlueprintNativeEvent )
	bool IsDead();

	UFUNCTION( BlueprintCallable, BlueprintNativeEvent )
	AActor* GetAvatar();

	// TODO: Change return value to Pointer
	UFUNCTION( BlueprintCallable, BlueprintNativeEvent )
	FTaggedMontage GetRandAttackMontage();

	// TODO: Change return value to Pointer
	UFUNCTION( BlueprintCallable, BlueprintNativeEvent )
	FTaggedMontage FindAttackMontageByTag( FGameplayTag MontageTag );

	UFUNCTION( BlueprintCallable, BlueprintNativeEvent )
	UNiagaraSystem* GetHurtNSEffect();

	UFUNCTION( BlueprintCallable, BlueprintNativeEvent )
	USoundBase* GetHurtSound();

	UFUNCTION( BlueprintCallable, BlueprintNativeEvent )
	int GetMinionsCount();

	UFUNCTION( BlueprintCallable, BlueprintNativeEvent )
	void AddMinionsCount( int Value );

	UFUNCTION( BlueprintCallable, BlueprintNativeEvent )
	void SetMasterActor( AActor* InMasterActor );
};

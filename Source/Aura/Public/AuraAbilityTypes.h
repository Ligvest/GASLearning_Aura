#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

class UGameplayEffect;

USTRUCT( BlueprintType )
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams() {}

	UPROPERTY( BlueprintReadWrite )
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY( BlueprintReadWrite )
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

	UPROPERTY( BlueprintReadWrite )
	TObjectPtr<UAbilitySystemComponent> SourceASC;

	UPROPERTY( BlueprintReadWrite )
	TObjectPtr<UAbilitySystemComponent> TargetASC;

	UPROPERTY( BlueprintReadWrite )
	float AbilityLevel = 1.f;

	UPROPERTY( BlueprintReadWrite )
	float BaseDamage = 0.f;

	UPROPERTY( BlueprintReadWrite )
	FGameplayTag DamageTypeTag = FGameplayTag();

	UPROPERTY( BlueprintReadWrite )
	float DebuffChance = 0.f;

	UPROPERTY( BlueprintReadWrite )
	float DebuffDamage = 0.f;

	UPROPERTY( BlueprintReadWrite )
	float DebuffDuration = 0.f;

	UPROPERTY( BlueprintReadWrite )
	float DebuffFrequency = 0.f;

	UPROPERTY( BlueprintReadWrite )
	float DeathImpulseMagnitude = 0.f;

	UPROPERTY( BlueprintReadWrite )
	FVector DeathImpulse = FVector::ZeroVector;

	UPROPERTY( BlueprintReadWrite )
	float KnockbackChance = 0.f;

	UPROPERTY( BlueprintReadWrite )
	float KnockbackMagnitude = 0.f;

	UPROPERTY( BlueprintReadWrite )
	FVector KnockbackImpulse = FVector::ZeroVector;

	UPROPERTY( BlueprintReadWrite )
	bool bIsRadialDamage = false;

	UPROPERTY( BlueprintReadWrite )
	float RadialDamageInnerRadius = 0.f;

	UPROPERTY( BlueprintReadWrite )
	float RadialDamageOuterRadius = 0.f;

	UPROPERTY( BlueprintReadWrite )
	FVector RadialDamageOrigin = FVector::ZeroVector;
};

USTRUCT()
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	// This is one way to pass data from ExecCalc to AttributeSet PostGameplayEffectExecute
	// But this is a stupid way as we still have access to GameplayEffectSpec from PostGameplayEffectExecute function
	// And from this Spec we easily can access all SetByCallerMagnitude data which already has all this shit
	// Also we can use SetByCallerMagnitude to pass a bool as a number.
	// So for learning purposes this is nice to try to make your custom EffectContext but in this situation you mustn't use it

	// Getters
	bool IsBlockedHit() const { return bIsBlockedHit; }
	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsDebuffSucceeded() const { return bDebuffSucceeded; }
	float GetDebuffDamage() const { return DebuffDamage; }
	float GetDebuffDuration() const { return DebuffDuration; }
	float GetDebuffFrequency() const { return DebuffFrequency; }
	FGameplayTag GetDamageTypeTag() const { return DamageTypeTag; }
	FVector GetDeathImpulse() const { return DeathImpulse; }
	FVector GetKnockbackImpulse() const { return KnockbackImpulse; }
	bool IsRadialDamage() const { return bIsRadialDamage; }
	float GetRadialDamageInnerRadius() const { return RadialDamageInnerRadius; }
	float GetRadialDamageOuterRadius() const { return RadialDamageOuterRadius; }
	FVector GetRadialDamageOrigin() const { return RadialDamageOrigin; }

	// Setters
	void SetIsBlockedHit( bool bInIsBlockedHit ) { bIsBlockedHit = bInIsBlockedHit; }
	void SetIsCriticalHit( bool bInIsCriticalHit ) { bIsCriticalHit = bInIsCriticalHit; }
	void SetDebuffSucceeded( bool bInDebuffSucceeded ) { bDebuffSucceeded = bInDebuffSucceeded; }
	void SetDebuffDamage( float InDebuffDamage ) { DebuffDamage = InDebuffDamage; }
	void SetDebuffDuration( float InDebuffDuration ) { DebuffDuration = InDebuffDuration; }
	void SetDebuffFrequency( float InDebuffFrequency ) { DebuffFrequency = InDebuffFrequency; }
	void SetDamageTypeTag( const FGameplayTag InDamageTypeTag ) { DamageTypeTag = InDamageTypeTag; }
	void SetDeathImpulse( const FVector InDeathImpulse ) { DeathImpulse = InDeathImpulse; }
	void SetKnockbackImpulse( const FVector InKnockbackImpulse ) { KnockbackImpulse = InKnockbackImpulse; }
	void SetIsRadialDamage( bool bInIsRadialDamage ) { bIsRadialDamage = bInIsRadialDamage; }
	void SetRadialDamageInnerRadius( float InRadialDamageInnerRadius ) { RadialDamageInnerRadius = InRadialDamageInnerRadius; }
	void SetRadialDamageOuterRadius( float InRadialDamageOuterRadius ) { RadialDamageOuterRadius = InRadialDamageOuterRadius; }
	void SetRadialDamageOrigin( const FVector& InRadialDamageOrigin ) { RadialDamageOrigin = InRadialDamageOrigin; }

	GENERATED_USTRUCT_BODY()
public:
	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const override { return StaticStruct(); }

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FAuraGameplayEffectContext* Duplicate() const
	{
		FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
		*NewContext = *this;
		if ( GetHitResult() )
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult( *GetHitResult(), true );
		}
		return NewContext;
	}

	/** Custom serialization, subclasses must override this */
	// Comment how to override the function
	virtual bool NetSerialize( FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess ) override;

protected:
	// This is one way to pass data from ExecCalc to AttributeSet PostGameplayEffectExecute
	// But this is a stupid way as we still have access to GameplayEffectSpec from PostGameplayEffectExecute function
	// And from this Spec we easily can access all SetByCallerMagnitude data which already has all this shit
	// Also we can use SetByCallerMagnitude to pass a bool as a number.
	// So for learning purposes this is nice to try to make your custom EffectContext but in this situation you mustn't use it

	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;

	UPROPERTY()
	bool bDebuffSucceeded = false;

	UPROPERTY()
	float DebuffDamage = 0.f;

	UPROPERTY()
	float DebuffDuration = 0.f;

	UPROPERTY()
	float DebuffFrequency = 0.f;

	// Using pointer is better here to save another 6 bytes of network bandwidth
	UPROPERTY()
	FGameplayTag DamageTypeTag;

	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector;

	UPROPERTY()
	FVector KnockbackImpulse = FVector::ZeroVector;

	UPROPERTY()
	bool bIsRadialDamage = false;

	UPROPERTY()
	float RadialDamageInnerRadius = 0.f;

	UPROPERTY()
	float RadialDamageOuterRadius = 0.f;

	UPROPERTY()
	FVector RadialDamageOrigin = FVector::ZeroVector;
};

// Overrided to have the same parameters for copying and serializing as FGameplayEffectContext
template <>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true  // Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};

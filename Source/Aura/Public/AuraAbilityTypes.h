#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

USTRUCT()
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	// Getters
	bool IsBlockedHit() const { return bIsBlockedHit; }
	bool IsCriticalHit() const { return bIsCriticalHit; }

	// Setters
	void SetIsBlockedHit( bool bInIsBlockedHit ) { bIsBlockedHit = bInIsBlockedHit; }
	void SetIsCriticalHit( bool bInIsCriticalHit ) { bIsCriticalHit = bInIsCriticalHit; }

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
	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;
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

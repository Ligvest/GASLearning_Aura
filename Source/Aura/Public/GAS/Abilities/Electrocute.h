// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/AuraBeamSpell.h"
#include "Electrocute.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UElectrocute : public UAuraBeamSpell
{
	GENERATED_BODY()
public:
	virtual FString GetDescription( int32 Level ) const override;
	virtual FString GetNextLevelDescription( int32 Level ) const override;
};

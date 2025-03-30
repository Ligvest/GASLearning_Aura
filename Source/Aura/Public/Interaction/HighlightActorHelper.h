// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"

UENUM( BlueprintType )
enum class EHighlightActorType : uint8
{
	None UMETA( DisplayName = "None" ),
	Enemy UMETA( DisplayName = "Enemy" ),
	Ally UMETA( DisplayName = "Ally" ),
	Interactable UMETA( DisplayName = "Interactable" )
};

/**
 *
 */
class AURA_API FHighlightActorHelper
{
public:
	static int HighlightActorTypeToStencilValue( const EHighlightActorType HighlightType );
};

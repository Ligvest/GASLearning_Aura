// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HighlightActorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE( MinimalAPI )
class UHighlightActorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class AURA_API IHighlightActorInterface
{
	GENERATED_BODY()
public:
	virtual void HighlightActor() = 0;
	virtual void UnHighlightActor() = 0;

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
};

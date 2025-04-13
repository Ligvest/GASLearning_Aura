// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "Characters/AuraCharacterBase.h"
#include "Interaction/HighlightActorInterface.h"
#include "AuraAICharacter.generated.h"

enum class EHighlightActorType : uint8;
/**
 *
 */
UCLASS()
class AURA_API AAuraAICharacter : public AAuraCharacterBase, public IHighlightActorInterface
{
	GENERATED_BODY()
public:
	//~ Begin of IHightlightActorInterface
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	//~ End of IHightlightActorInterface

	virtual void BeginPlay() override;

	UPROPERTY( EditAnywhere, Category = Interaction )
	EHighlightActorType HighlightActorType;
};

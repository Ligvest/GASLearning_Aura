// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "Characters/AuraCharacterBase.h"
#include "Interaction/HighlightActorInterface.h"
#include "AuraAICharacter.generated.h"

class UBehaviorTree;
class AAuraAIController;
enum class EHighlightActorType : uint8;
/**
 *
 */
UCLASS()
class AURA_API AAuraAICharacter : public AAuraCharacterBase, public IHighlightActorInterface
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	virtual void PossessedBy( AController* NewController ) override;

	//~ Begin of IHightlightActorInterface
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	//~ End of IHightlightActorInterface

	UFUNCTION( BlueprintImplementableEvent )
	void SpawnLoot();

protected:
	UPROPERTY( EditAnywhere, Category = Interaction )
	EHighlightActorType HighlightActorType;
};

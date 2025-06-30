// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AuraAIController.generated.h"

class UBehaviorTreeComponent;
/**
 *
 */
UCLASS()
class AURA_API AAuraAIController : public AAIController
{
	GENERATED_BODY()

public:
	AAuraAIController();

protected:
	/*
	 * this component exists in AIController by default so don't create the second one
	UPROPERTY( EditDefaultsOnly )
	TObjectPtr<UBlackboardComponent> BlackboardComp;
	*/

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};

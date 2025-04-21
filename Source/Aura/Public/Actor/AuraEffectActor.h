// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"

// Forward declarations:
class USphereComponent;

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()

public:
	AAuraEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION( BlueprintCallable )
	void ApplyEffectToTarget( AActor* TargetActor ) const;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	TSubclassOf<UGameplayEffect> GameplayEffectClass;
};

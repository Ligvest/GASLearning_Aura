// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "DebuffNiagaraComponent.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UDebuffNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()

public:
	UDebuffNiagaraComponent();
	virtual void BeginPlay() override;

protected:
	void OnDebuffTagAddedOrRemoved( const FGameplayTag ChangedTag, int32 NewCount );

public:
	UPROPERTY( EditDefaultsOnly )
	FGameplayTag DebuffTag;
};

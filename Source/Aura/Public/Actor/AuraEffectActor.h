// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"

// Forward declarations:
class USphereComponent;

UENUM( BlueprintType )
enum class EEffectApplicationPolicy : uint8
{
	ApplyOnBeginOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM( BlueprintType )
enum class EEffectRemovalPolicy : uint8
{
	RemoveOnBeginOverlap,
	RemoveOnEndOverlap,
	DoNotRemove
};

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()

	// TODO: It might be better to rewrite logic to component system. To have an EffectActor empty and have component which could be configured for Applying, Removing, etc.
public:
	AAuraEffectActor();

protected:
	virtual void BeginPlay() override;

	void ApplyEffectToTarget( AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass );
	void RemoveInfiniteEffectFromTarget( AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass );

	UFUNCTION( BlueprintCallable )
	void OnBeginOverlap( AActor* TargetActor );
	UFUNCTION( BlueprintCallable )
	void OnEndOverlap( AActor* TargetActor );

	// Instant gameplay effect
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Effects Preferences | Instant Effect" )
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;
	UPROPERTY( EditDefaultsOnly, Category = "Effects Preferences | Instant Effect" )
	EEffectApplicationPolicy InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	// Duration gameplay effect
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Effects Preferences | Duration Effect" )
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;
	UPROPERTY( EditDefaultsOnly, Category = "Effects Preferences | Duration Effect" )
	EEffectApplicationPolicy DurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	// Infinite gameplay effect
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Effects Preferences | Infinite Effect" )
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;
	UPROPERTY( EditDefaultsOnly, Category = "Effects Preferences | Infinite Effect" )
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	UPROPERTY( EditDefaultsOnly, Category = "Effects Preferences | Infinite Effect" )
	EEffectRemovalPolicy InfiniteEffectRemovalPolicy = EEffectRemovalPolicy::DoNotRemove;
	TMap<UAbilitySystemComponent*, FActiveGameplayEffectHandle> AscToInfiniteGEHandle;

	UPROPERTY( EditAnywhere, Category = "Effects Preferences" )
	float EffectLevel = 1.f;
};

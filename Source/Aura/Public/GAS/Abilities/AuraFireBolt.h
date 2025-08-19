// Dovzhik Tolya

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/AuraProjectileSpell.h"
#include "AuraFireBolt.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UAuraFireBolt : public UAuraProjectileSpell
{
	GENERATED_BODY()
public:
	virtual FString GetDescription( int32 Level ) const override;
	virtual FString GetNextLevelDescription( int32 Level ) const override;

	UFUNCTION( BlueprintCallable )
	void SpawnProjectiles( const FVector ProjectileTargetLocation, const FGameplayTag SocketTag, bool bOverridePitch, float PitchOverride, AActor* HomingTarget );

protected:
	UPROPERTY( EditDefaultsOnly, Category = "FireBolt" )
	float ProjectileSpread = 90.f;

	UPROPERTY( EditDefaultsOnly )
	int32 NumProjectiles = 5;

	UPROPERTY( EditDefaultsOnly, Category = "FireBolt" )
	int32 MaxNumProjectiles = 5;

	UPROPERTY( EditDefaultsOnly, Category = "FireBolt" )
	float HomingAccelerationMin = 1600.f;

	UPROPERTY( EditDefaultsOnly, Category = "FireBolt" )
	float HomingAccelerationMax = 3200.f;

	UPROPERTY( EditDefaultsOnly, Category = "FireBolt" )
	bool bLaunchHomingProjectiles = true;
};

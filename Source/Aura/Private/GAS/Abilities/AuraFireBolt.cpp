// Dovzhik Tolya

#include "GAS/Abilities/AuraFireBolt.h"

#include "AuraGameplayTags.h"
#include "Actor/AuraProjectile.h"
#include "GAS/AuraGasBpLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interaction/CombatInterface.h"

FString UAuraFireBolt::GetDescription( int32 Level ) const
{
	const int32 Damage = DamageScalableFloat.GetValueAtLevel( Level );
	const float ManaCost = FMath::Abs( GetManaCost( Level ) );
	const float Cooldown = GetCooldown( Level );
	return FString::Printf( TEXT(
	                            // Title
	                            "<Title>Fire Bolt</>\n"
	                            // Level
	                            "<Small>Level: </><Level>%d</>\n"
	                            // ManaCost
	                            "<Small>ManaCost: </><ManaCost>%.1f</>\n"
	                            // Cooldown
	                            "<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

	                            // Number of FireBolts
	                            "<Default>Launches %d bolts of fire, "
	                            "exploding on impact and dealing: </>"
	                            // Damage
	                            "<Damage>%d</><Default> fire damage with"
	                            " a chance to burn</>" ),

	                        // Values
	                        Level, ManaCost, Cooldown, FMath::Min( Level, NumProjectiles ), Damage );
}

FString UAuraFireBolt::GetNextLevelDescription( int32 Level ) const
{
	const int32 Damage = DamageScalableFloat.GetValueAtLevel( Level );
	const float ManaCost = FMath::Abs( GetManaCost( Level ) );
	const float Cooldown = GetCooldown( Level );
	return FString::Printf( TEXT(
	                            // Title
	                            "<Title>Next Level</>\n"
	                            // Level
	                            "<Small>Level: </><Level>%d</>\n"
	                            // ManaCost
	                            "<Small>ManaCost: </><ManaCost>%.1f</>\n"
	                            // Cooldown
	                            "<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

	                            // Number of FireBolts
	                            "<Default>Launches %d bolts of fire, "
	                            "exploding on impact and dealing: </>"
	                            // Damage
	                            "<Damage>%d</><Default> fire damage with"
	                            " a chance to burn</>" ),

	                        // Values
	                        Level, ManaCost, Cooldown, FMath::Min( Level, NumProjectiles ), Damage );
}

void UAuraFireBolt::SpawnProjectiles( const FVector ProjectileTargetLocation, const FGameplayTag SocketTag, bool bOverridePitch, float PitchOverride, AActor* HomingTarget )
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if ( !bIsServer ) return;

	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation( GetAvatarActorFromActorInfo(), SocketTag );
	FRotator Rotation = ( ProjectileTargetLocation - SocketLocation ).Rotation();
	if ( bOverridePitch ) Rotation.Pitch = PitchOverride;

	const FVector Forward = Rotation.Vector();
	const int32 EffectiveNumProjectiles = FMath::Min( NumProjectiles, GetAbilityLevel() );
	TArray<FRotator> Rotations = UAuraGasBpLibrary::EvenlySpacedRotators( Forward, FVector::UpVector, ProjectileSpread, EffectiveNumProjectiles );

	for ( const FRotator& Rot : Rotations )
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation( SocketLocation );
		SpawnTransform.SetRotation( Rot.Quaternion() );

		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>( ProjectileClass, SpawnTransform, GetAvatarActorFromActorInfo(), Cast<APawn>( GetOwningActorFromActorInfo() ),
		                                                                               ESpawnActorCollisionHandlingMethod::AlwaysSpawn );

		Projectile->ImpactEffectParams = MakeDamageEffectParamsFromClassDefaults();

		// If the target is a real character
		if ( HomingTarget && HomingTarget->Implements<UCombatInterface>() )
		{
			Projectile->MovementComponent->HomingTargetComponent = HomingTarget->GetRootComponent();
		}
		// If the target is some mesh, wall, floor, etc.
		else
		{
			// Making USceneComponent and use Projectile as Outer so the component would be destroyed together with the Projectile
			USceneComponent* HomingSceneComponent = NewObject<USceneComponent>( Projectile );
			HomingSceneComponent->SetWorldLocation( ProjectileTargetLocation );
			Projectile->MovementComponent->HomingTargetComponent = HomingSceneComponent;
		}

		Projectile->MovementComponent->HomingAccelerationMagnitude = FMath::FRandRange( HomingAccelerationMin, HomingAccelerationMax );
		Projectile->MovementComponent->bIsHomingProjectile = bLaunchHomingProjectiles;

		Projectile->FinishSpawning( SpawnTransform );
	}
}
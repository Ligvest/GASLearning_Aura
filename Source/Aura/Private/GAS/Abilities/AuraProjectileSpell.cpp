// Dovzhik Tolya

#include "GAS/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemComponent.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "AuraAbilityTypes.h"

void UAuraProjectileSpell::ActivateAbility( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                            const FGameplayEventData* TriggerEventData )
{
	Super::ActivateAbility( Handle, ActorInfo, ActivationInfo, TriggerEventData );
}
void UAuraProjectileSpell::SpawnProjectile( FVector TargetLocation, const FGameplayTag SpawnCombatSocketTag )
{
	// Activate only if this is a server
	// So return if this is a client
	if ( !GetAvatarActorFromActorInfo()->HasAuthority() )
	{
		return;
	}

	AActor* Owner = GetOwningActorFromActorInfo();
	APawn* Instigator = Cast<APawn>( Owner );
	AActor* Avatar = GetAvatarActorFromActorInfo();
	check( Avatar->Implements<UCombatInterface>() );
	const FVector SpawnLocation = ICombatInterface::Execute_GetCombatSocketLocation( Avatar, SpawnCombatSocketTag );

	FRotator Rotator = ( TargetLocation - SpawnLocation ).Rotation();
	// Zero out pitch to make projectile fly orthogonal to surface
	Rotator.Pitch = 0.f;

	FTransform SpawnTransform;
	SpawnTransform.SetLocation( SpawnLocation );
	SpawnTransform.SetRotation( Rotator.Quaternion() );

	AAuraProjectile* SpawnedProjectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>( ProjectileClass, SpawnTransform, Owner, Instigator, ESpawnActorCollisionHandlingMethod::AlwaysSpawn );

	FDamageEffectParams TempEffectParams = MakeDamageEffectParamsFromClassDefaults();
	SpawnedProjectile->SetImpactEffectParams( MoveTemp( TempEffectParams ) );
	SpawnedProjectile->FinishSpawning( SpawnTransform );
}

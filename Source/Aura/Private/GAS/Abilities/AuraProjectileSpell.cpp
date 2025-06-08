// Dovzhik Tolya

#include "GAS/Abilities/AuraProjectileSpell.h"

#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

void UAuraProjectileSpell::ActivateAbility( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                            const FGameplayEventData* TriggerEventData )
{
	Super::ActivateAbility( Handle, ActorInfo, ActivationInfo, TriggerEventData );

	// Activate only if this is a serve
	if ( !HasAuthority( &ActivationInfo ) )
	{
		return;
	}

	AActor* Owner = GetOwningActorFromActorInfo();
	APawn* Instigator = Cast<APawn>( Owner );
	AActor* Avatar = GetAvatarActorFromActorInfo();
	ICombatInterface* CombatActor = Cast<ICombatInterface>( Avatar );
	FVector SpawnLocation = FVector::ZeroVector;
	if ( CombatActor )
	{
		SpawnLocation = CombatActor->GetProjectileSpawnSocketLocation();
	}

	FTransform SpawnTransform;
	SpawnTransform.SetLocation( SpawnLocation );

	// TODO: Set projectile rotation

	AActor* SpawnedProjectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>( ProjectileClass, SpawnTransform, Owner, Instigator, ESpawnActorCollisionHandlingMethod::AlwaysSpawn );

	// TODO: Assign gameplay effect to damage

	SpawnedProjectile->FinishSpawning( SpawnTransform );
}
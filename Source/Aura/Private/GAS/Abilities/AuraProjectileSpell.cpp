// Dovzhik Tolya

#include "GAS/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemComponent.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

void UAuraProjectileSpell::ActivateAbility( const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                            const FGameplayEventData* TriggerEventData )
{
	Super::ActivateAbility( Handle, ActorInfo, ActivationInfo, TriggerEventData );
}
void UAuraProjectileSpell::SpawnProjectile( FVector TargetLocation )
{
	// Activate only if this is a server
	if ( !GetAvatarActorFromActorInfo()->HasAuthority() )
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

	FRotator Rotator = ( TargetLocation - SpawnLocation ).Rotation();
	// Zero out pitch to make projectile fly ortogonal to surface
	Rotator.Pitch = 0.f;

	FTransform SpawnTransform;
	SpawnTransform.SetLocation( SpawnLocation );
	SpawnTransform.SetRotation( Rotator.Quaternion() );

	// TODO: Set projectile rotation

	AAuraProjectile* SpawnedProjectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>( ProjectileClass, SpawnTransform, Owner, Instigator, ESpawnActorCollisionHandlingMethod::AlwaysSpawn );

	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	check( SourceASC );
	FGameplayEffectSpecHandle ImpactEffectHandle = SourceASC->MakeOutgoingSpec( ImpactEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext() );
	SpawnedProjectile->SetImpactEffectHandle( std::move( ImpactEffectHandle ) );

	SpawnedProjectile->FinishSpawning( SpawnTransform );
}
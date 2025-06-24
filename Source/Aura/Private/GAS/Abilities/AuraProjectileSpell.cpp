// Dovzhik Tolya

#include "GAS/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
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

	AAuraProjectile* SpawnedProjectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>( ProjectileClass, SpawnTransform, Owner, Instigator, ESpawnActorCollisionHandlingMethod::AlwaysSpawn );

	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	check( SourceASC );
	float AbilityLevel = GetAbilityLevel();

	// Example of things we can add to EffectContextHandle to get them later.
	// E.g. in AttributeSet functions such as PostGameplayEffectExecute or PreAttributeChange
	FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
	EffectContextHandle.SetAbility( this );
	EffectContextHandle.AddSourceObject( SpawnedProjectile );
	TArray<TWeakObjectPtr<AActor>> Actors;
	Actors.Add( SpawnedProjectile );
	EffectContextHandle.AddActors( Actors );
	FHitResult HitResult;
	HitResult.Location = TargetLocation;
	EffectContextHandle.AddHitResult( HitResult );

	FGameplayEffectSpecHandle ImpactEffectSpecHandle = SourceASC->MakeOutgoingSpec( ImpactEffectClass, AbilityLevel, EffectContextHandle );

	const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();
	float DamageValue = Magnitude.GetValueAtLevel( 20 );
	ImpactEffectSpecHandle.Data->SetSetByCallerMagnitude( AuraTags.Values_Damage, DamageValue );
	SpawnedProjectile->SetImpactEffectHandle( std::move( ImpactEffectSpecHandle ) );

	SpawnedProjectile->FinishSpawning( SpawnTransform );
}
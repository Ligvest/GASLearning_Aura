// Dovzhik Tolya

#include "Actor/AuraProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Aura/Aura.h"
#include "Components/AudioComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GAS/AuraGasBpLibrary.h"

AAuraProjectile::AAuraProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	SetReplicates( true );

	CollisionSphere = CreateDefaultSubobject<USphereComponent>( TEXT( "CollisionSphere" ) );
	SetRootComponent( CollisionSphere );

	CollisionSphere->SetCollisionObjectType( ECC_Projectile );

	// Disable blocking collisions and leave only QueryOnly type ( like Overlap, LineTrace )
	CollisionSphere->SetCollisionEnabled( ECollisionEnabled::QueryOnly );

	// Disable Collision response for all channels
	CollisionSphere->SetCollisionResponseToAllChannels( ECR_Ignore );

	// And enable Overlap only for Dynamic, Static and Pawn channels
	CollisionSphere->SetCollisionResponseToChannel( ECC_WorldDynamic, ECR_Overlap );
	CollisionSphere->SetCollisionResponseToChannel( ECC_WorldStatic, ECR_Overlap );
	CollisionSphere->SetCollisionResponseToChannel( ECC_Pawn, ECR_Overlap );

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>( TEXT( "MovementComponent" ) );
	MovementComponent->InitialSpeed = 550.f;
	MovementComponent->MaxSpeed = 550.f;
	MovementComponent->ProjectileGravityScale = 0.f;
}

void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement( true );
	SetLifeSpan( LifeSpan );
	CollisionSphere->OnComponentBeginOverlap.AddDynamic( this, &AAuraProjectile::OnCollisionSphereOverlap );

	// Make fly sound for clients or ListenServer
	FlySoundComponent = UGameplayStatics::SpawnSoundAttached( FlySound, GetRootComponent() );
}

void AAuraProjectile::PlayImpactEffects() const
{
	/*
	 * Can't use checks here as this function is called in Destroyed method
	 * And Destroyed method is called during compilation in BP when the engine tries to
	 * remove all actors of this type from world and create new ones if necessary
	check( ImpactEffect );
	check( ImpactSound );
	*/

	UGameplayStatics::PlaySoundAtLocation( this, ImpactSound, GetActorLocation() );
	UNiagaraFunctionLibrary::SpawnSystemAtLocation( this, ImpactEffect, GetActorLocation() );

	if ( IsValid( FlySoundComponent ) )
	{
		FlySoundComponent->Stop();
		FlySoundComponent->DestroyComponent();
	}
}
bool AAuraProjectile::IsValidOverlap( AActor* OtherActor )
{
	// We use GetOwner instad of SourceASC as SourceASC isn't replicated but owner does
	// AActor* AttackerActor = ImpactEffectParams.SourceASC->GetAvatarActor();
	AActor* AttackerActor = GetOwner();
	const bool bCauserSameAsTarget = AttackerActor == OtherActor;
	if ( bCauserSameAsTarget )
	{
		return false;
	}

	const FName OpponentTag = UAuraGasBpLibrary::GetOpponentActorTag( AttackerActor );
	if ( !OtherActor->ActorHasTag( OpponentTag ) )
	{
		return false;
	}

	return true;
}

void AAuraProjectile::OnCollisionSphereOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                const FHitResult& SweepResult )
{
	// If this is a client the we will get nullptr in ImpactEffectHandle as we set it only on server
	if ( !HasAuthority() )
	{
		return;
	}

	if ( !IsValidOverlap( OtherActor ) )
	{
		return;
	}

	if ( !bImpactHappened )
	{
		bImpactHappened = true;

		// Show impact effects to clients or ListenServer
		PlayImpactEffects();

		// Only server destroys the projectile
		// if ( HasAuthority() )
		// {

		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent( OtherActor );
		if ( TargetASC )
		{
			// Probably better solution would be to just pass location of the sphere or something like this
			// and calculate all impulses there to not pass all these vectors
			const FVector DeathImpulse = GetActorForwardVector() * ImpactEffectParams.DeathImpulseMagnitude;
			ImpactEffectParams.DeathImpulse = DeathImpulse;
			const bool bKnockbackSucceeded = FMath::RandRange( 1, 100 ) < ImpactEffectParams.KnockbackChance;
			if ( bKnockbackSucceeded )
			{
				FRotator Rotation = GetActorRotation();
				Rotation.Pitch = 45.f;

				const FVector KnockbackDirection = Rotation.Vector();
				const FVector KnockbackImpulse = KnockbackDirection * ImpactEffectParams.KnockbackMagnitude;
				ImpactEffectParams.KnockbackImpulse = KnockbackImpulse;
			}

			ImpactEffectParams.TargetASC = TargetASC;
			UAuraGasBpLibrary::ApplyDamageEffect( ImpactEffectParams );
		}

		Destroy();
		// }
	}
}

void AAuraProjectile::Destroyed()
{
	// Only for clients which didn't see the effect
	// And for ListenServer if the projectile was destroyed without overlap ( e.g. by LifeSpan )
	if ( !bImpactHappened )
	{
		bImpactHappened = true;
		PlayImpactEffects();
	}

	Super::Destroyed();
}

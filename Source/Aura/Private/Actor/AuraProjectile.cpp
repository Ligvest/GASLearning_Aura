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

AAuraProjectile::AAuraProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

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
	SetLifeSpan( LifeSpan );
	CollisionSphere->OnComponentBeginOverlap.AddDynamic( this, &AAuraProjectile::OnCollisionSphereOverlap );

	// Make fly sound for clients or ListenServer
	FlySoundComponent = UGameplayStatics::SpawnSoundAttached( FlySound, GetRootComponent() );
}

void AAuraProjectile::PlayImpactEffects() const
{
	check( ImpactEffect );
	check( ImpactSound );

	UGameplayStatics::PlaySoundAtLocation( this, ImpactSound, GetActorLocation() );
	UNiagaraFunctionLibrary::SpawnSystemAtLocation( this, ImpactEffect, GetActorLocation() );

	if ( IsValid( FlySoundComponent ) )
	{
		FlySoundComponent->Stop();
		FlySoundComponent->DestroyComponent();
	}
}

void AAuraProjectile::OnCollisionSphereOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                const FHitResult& SweepResult )
{
	if ( !bImpactHappened )
	{
		bImpactHappened = true;

		// Show impact effects to clients or ListenServer
		PlayImpactEffects();

		// Only server destroys the projectile
		if ( HasAuthority() )
		{
			UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent( OtherActor );
			if ( TargetASC )
			{
				TargetASC->ApplyGameplayEffectSpecToSelf( *ImpactEffectHandle.Data );
			}

			Destroy();
		}
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

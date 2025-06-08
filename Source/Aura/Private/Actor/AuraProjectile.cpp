// Dovzhik Tolya

#include "Actor/AuraProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AAuraProjectile::AAuraProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	CollisionSphere = CreateDefaultSubobject<USphereComponent>( TEXT( "CollisionSphere" ) );
	SetRootComponent( CollisionSphere );
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

void AAuraProjectile::OnCollisionSphereOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                const FHitResult& SweepResult )
{
}

void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	CollisionSphere->OnComponentBeginOverlap.AddDynamic( this, &AAuraProjectile::OnCollisionSphereOverlap );
}

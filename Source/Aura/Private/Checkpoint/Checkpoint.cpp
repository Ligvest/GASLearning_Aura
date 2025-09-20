// Dovzhik Tolya

#include "Checkpoint/Checkpoint.h"

#include "Components/SphereComponent.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/HighlightActorHelper.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

ACheckpoint::ACheckpoint( const FObjectInitializer& ObjectInitializer ) : Super( ObjectInitializer )
{
	PrimaryActorTick.bCanEverTick = false;

	MoveToComponent = CreateDefaultSubobject<UStaticMeshComponent>( "MoveToComponent" );
	MoveToComponent->SetupAttachment( GetRootComponent() );

	CheckpointMesh = CreateDefaultSubobject<UStaticMeshComponent>( "CheckpointMesh" );
	CheckpointMesh->SetupAttachment( GetRootComponent() );
	CheckpointMesh->SetCollisionEnabled( ECollisionEnabled::QueryAndPhysics );
	CheckpointMesh->SetCollisionResponseToAllChannels( ECR_Block );

	Sphere = CreateDefaultSubobject<USphereComponent>( "Sphere" );
	Sphere->SetupAttachment( CheckpointMesh );
	Sphere->SetCollisionEnabled( ECollisionEnabled::QueryOnly );
	Sphere->SetCollisionResponseToAllChannels( ECR_Ignore );
	Sphere->SetCollisionResponseToChannel( ECC_Pawn, ECR_Overlap );
}

void ACheckpoint::LoadActor_Implementation()
{
	if ( bReached )
	{
		HandleGlowEffects();
	}
}

void ACheckpoint::HighlightActor()
{
	if ( bReached )
	{
		return;
	}

	CheckpointMesh->SetRenderCustomDepth( true );
	CheckpointMesh->MarkRenderStateDirty();
}

void ACheckpoint::UnHighlightActor()
{
	if ( bReached )
	{
		return;
	}

	CheckpointMesh->SetRenderCustomDepth( false );
	CheckpointMesh->MarkRenderStateDirty();
}

void ACheckpoint::SetMoveToLocation_Implementation( FVector& OutDestination )
{
	OutDestination = MoveToComponent->GetComponentLocation();
}

void ACheckpoint::OnSphereOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult )
{
	if ( OtherActor->Implements<UPlayerInterface>() )
	{
		UnHighlightActor();
		bReached = true;

		if ( AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>( UGameplayStatics::GetGameMode( this ) ) )
		{
			const UWorld* World = GetWorld();
			FString MapName = World->GetMapName();
			MapName.RemoveFromStart( World->StreamingLevelsPrefix );

			AuraGM->SaveWorldState( GetWorld(), MapName );
		}

		IPlayerInterface::Execute_SaveProgress( OtherActor, PlayerStartTag );
		HandleGlowEffects();
	}
}

void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic( this, &ACheckpoint::OnSphereOverlap );
	const int DepthStencilValue = FHighlightActorHelper::HighlightActorTypeToStencilValue( HighlightActorType );
	CheckpointMesh->SetCustomDepthStencilValue( DepthStencilValue );
}

void ACheckpoint::HandleGlowEffects()
{
	Sphere->SetCollisionEnabled( ECollisionEnabled::NoCollision );
	UMaterialInstanceDynamic* DynamicMaterialInstace = UMaterialInstanceDynamic::Create( CheckpointMesh->GetMaterial( 0 ), this );
	CheckpointMesh->SetMaterial( 0, DynamicMaterialInstace );
	CheckpointReached( DynamicMaterialInstace );
}
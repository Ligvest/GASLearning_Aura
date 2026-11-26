// Dovzhik Tolya

#include "Checkpoint/MapEntrance.h"

#include "Components/SphereComponent.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

AMapEntrance::AMapEntrance( const FObjectInitializer& ObjectInitializer ) : Super( ObjectInitializer )
{
	Sphere->SetupAttachment( MoveToComponent );
}

void AMapEntrance::HighlightActor()
{
	/*
	CheckpointMesh->SetRenderCustomDepth( true );
	CheckpointMesh->MarkRenderStateDirty();
	*/
	SetLightEnabled( true );
}

void AMapEntrance::UnHighlightActor()
{
	/*
	CheckpointMesh->SetRenderCustomDepth( false );
	CheckpointMesh->MarkRenderStateDirty();
	*/
	SetLightEnabled( false );
}

void AMapEntrance::LoadActor_Implementation()
{
	// Do nothing when loading a Map Entrance
}

void AMapEntrance::OnSphereOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult )
{
	// Overlaps fire on clients too. Saving and travelling are the server's job
	if ( !HasAuthority() || bTravelStarted )
	{
		return;
	}

	if ( OtherActor->Implements<UPlayerInterface>() )
	{
		AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>( UGameplayStatics::GetGameMode( this ) );
		if ( !AuraGM )
		{
			return;
		}

		bReached = true;
		bTravelStarted = true;

		AuraGM->SaveWorldState( GetWorld(), DestinationMap.ToSoftObjectPath().GetAssetName() );
		// Whoever entered, the host's progress is saved ( remote players are skipped inside SaveProgress )
		AuraGM->SaveProgressForAllPlayers( DestinationPlayerStartTag );
		// ServerTravel: every connected client goes to the new map together with the host
		AuraGM->TravelToMap( DestinationMap );
	}
}
// Dovzhik Tolya

#include "GAS/Abilities/AuraSummonAbility.h"

#include "NavigationSystem.h"
#include "Algo/RandomShuffle.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
	TArray<FVector> SpawnLocations;

	AActor* Avatar = GetAvatarActorFromActorInfo();
	const FVector ActorForward = Avatar->GetActorForwardVector();
	const FVector ActorLocation = Avatar->GetActorLocation();
	const float SpreadAngleDelta = NumMinions > 1 ? SpawnSpreadAngle / ( NumMinions - 1 ) : 0.f;
	const FVector LeftOfSpread = NumMinions > 1 ? ActorForward.RotateAngleAxis( -SpawnSpreadAngle / 2.f, FVector::UpVector ) : ActorForward;

	// Project the caster onto the NavMesh once. All rays start from this point.
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent( GetWorld() );
	FNavLocation StartNav;
	const bool bHasNav = NavSys && NavSys->ProjectPointToNavigation( ActorLocation, StartNav );

	for ( int32 i = 0; i < NumMinions; i++ )
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis( SpreadAngleDelta * i, FVector::UpVector );
		const float Distance = FMath::FRandRange( MinSpawnDistance, MaxSpawnDistance );
		FVector SpawnLocation = ActorLocation + Direction * Distance;

		if ( bHasNav )
		{
			// Raycast along the NavMesh. If the ray hits the NavMesh boundary (wall, map edge),
			// spawn at the hit point pulled back a little, so minions never end up behind obstacles.
			FVector HitLocation;
			if ( UNavigationSystemV1::NavigationRaycast( GetWorld(), StartNav.Location, SpawnLocation, HitLocation ) )
			{
				SpawnLocation = HitLocation - Direction * SpawnEdgeOffset;
			}

			// Snap the final point onto the NavMesh surface (handles height differences).
			FNavLocation ProjectedLocation;
			if ( NavSys->ProjectPointToNavigation( SpawnLocation, ProjectedLocation ) )
			{
				SpawnLocation = ProjectedLocation.Location;
			}
		}

		SpawnLocations.Add( SpawnLocation );
	}

	Algo::RandomShuffle( SpawnLocations );
	return SpawnLocations;
}
TSubclassOf<APawn> UAuraSummonAbility::GetRandMinionClass()
{
	if ( MinionClasses.Num() <= 0 )
	{
		check( false );
		return nullptr;
	}

	int Index = FMath::RandRange( 0, MinionClasses.Num() - 1 );
	return MinionClasses[Index];
}
